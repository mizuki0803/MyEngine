#include "FallSphere.h"
#include "Input.h"

FallSphere* FallSphere::Create(Model* model)
{
	//�C���X�^���X�𐶐�
	FallSphere* instance = new FallSphere();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

FallSphere::~FallSphere()
{
	//�I�u�W�F�N�g���
	delete fallSphereObject;
}

bool FallSphere::Initialize(Model* model)
{
	//�������I�u�W�F�N�g����
	fallSphereObject = Object3d::Create(model);
	if (fallSphereObject == nullptr) {
		return false;
	}

	//�����n�_�Ƒ傫�����Z�b�g
	fallSphereObject->SetPosition(startPos);
	XMFLOAT3 scale = { 2, 2, 2 };
	fallSphereObject->SetScale(scale);

	return true;
}

void FallSphere::Update()
{
	//���͂̃C���X�^���X���擾
	Input* input = Input::GetInstance();

	//R�L�[���͂Ń��Z�b�g
	if (input->TriggerKey(DIK_R))
	{
		Reset();
	}

	//�n�ʂɂ��Ă��Ȃ���Ώd�͂ɂ�闎�����s��
	if (!isOnGround)
	{
		//���R����
		velY += gravity;

		//�������ɏd�͂�����������
		XMFLOAT3 pos = fallSphereObject->GetPosition();
		pos.y -= velY;

		//���W��0�ȉ��ɂȂ�����n�ʂɒ��������Ƃɂ���
		if (pos.y <= 0)
		{
			isOnGround = true;

			//�����߂�
			pos.y = 0;
		}

		//�X�V�������W���Z�b�g
		fallSphereObject->SetPosition(pos);
	}

	//�I�u�W�F�N�g�X�V
	fallSphereObject->Update();
}

void FallSphere::Draw()
{
	//�I�u�W�F�N�g�`��
	fallSphereObject->Draw();
}

void FallSphere::Reset()
{
	//�����n�_�ɖ߂�
	fallSphereObject->SetPosition(startPos);
	//���x��������
	velY = 0;
	//�n�ʂɂ��Ă��Ȃ�
	isOnGround = false;
}
