#include "Reticle.h"

Reticle* Reticle::Create(ObjModel* model)
{
	//���e�B�N���̃C���X�^���X�𐶐�
	Reticle* reticle = new Reticle();
	if (reticle == nullptr) {
		return nullptr;
	}

	// ������
	if (!reticle->Initialize()) {
		delete reticle;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	reticle->model = model;

	return reticle;
}

void Reticle::Update()
{
	//���@���烌�e�B�N���܂ł̋���
	const float distancePlayerToReticle = 25.0f;
	//���@����3D���e�B�N���ւ̃I�t�Z�b�gZ����
	Vector3 offset = { 0, 0, 1.0f };
	//���@�̃��[���h�s��̉�]�����e�B�N���ɔ��f
	offset = MatrixTransformDirection(offset, followMatWorld);
	//�x�N�g���̒����𐮂���
	offset = offset.normalize() * distancePlayerToReticle;
	position = {
		followMatWorld.r[3].m128_f32[0] + offset.x,
	followMatWorld.r[3].m128_f32[1] + offset.y,
	followMatWorld.r[3].m128_f32[2] + offset.z, };

	//3D���e�B�N���̍��W��ݒ�
	XMMATRIX matScale, matRot, matTrans;
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̍���
	matWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	matWorld *= matScale;	//���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot;		//���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f


	const XMMATRIX matViewProjection = camera->GetMatView() * camera->GetMatProjection();
	const Vector3 cameraPos = camera->GetEye();

	//�萔�o�b�t�@�ւ̃f�[�^�]��
	ConstBufferDataB0* constMap = nullptr;
	if (SUCCEEDED(constBuffB0->Map(0, nullptr, (void**)&constMap)))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = matWorld;
		constMap->cameraPos = cameraPos;
		constBuffB0->Unmap(0, nullptr);
	}
}

Vector3 Reticle::GetWorldPos()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���s�ړ��������擾
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}
