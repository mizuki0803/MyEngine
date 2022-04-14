#include "Turret.h"
#include "Input.h"

Turret* Turret::Create(Model* turretModel, Model* bulletModel)
{
	//�C���X�^���X�𐶐�
	Turret* instance = new Turret();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(turretModel, bulletModel)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Turret::~Turret()
{
	//�I�u�W�F�N�g���
	delete turretObject;
	delete bulletObject;
}

bool Turret::Initialize(Model* turretModel, Model* bulletModel)
{
	//�C��I�u�W�F�N�g����
	turretObject = Object3d::Create(turretModel);
	if (turretObject == nullptr) {
		return false;
	}

	//�����n�_�Ƒ傫���Ɖ�]���Z�b�g
	turretObject->SetPosition(startPos);
	XMFLOAT3 turretScale = { 4, 4, 4 };
	turretObject->SetScale(turretScale);
	turretObject->SetRotation({ 0, 90, 0 });


	//�e�I�u�W�F�N�g����
	bulletObject = Object3d::Create(bulletModel);
	if (bulletObject == nullptr) {
		return false;
	}

	//�����n�_�Ƒ傫�����Z�b�g
	bulletObject->SetPosition(startPos);
	XMFLOAT3 bulletScale = { 2, 2, 2 };
	bulletObject->SetScale(bulletScale);

	return true;
}

void Turret::Update()
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
		XMFLOAT3 pos = bulletObject->GetPosition();

		//X�����ړ�(���������^��)
		pos.x += vel.x;

		//���R����
		vel.y += gravity;
		//�e�ɏd�͂�����������
		pos.y -= vel.y;

		//���W��0�ȉ��ɂȂ�����n�ʂɒ��������Ƃɂ���
		if (pos.y <= 0)
		{
			isOnGround = true;

			//�����߂�
			pos.y = 0;
		}

		//�X�V�������W���Z�b�g
		bulletObject->SetPosition(pos);
	}

	//�I�u�W�F�N�g�X�V
	turretObject->Update();
	bulletObject->Update();
}

void Turret::Draw()
{
	//�I�u�W�F�N�g�`��
	turretObject->Draw();
	bulletObject->Draw();
}

void Turret::Reset()
{
	//�����n�_�ɖ߂�
	bulletObject->SetPosition(startPos);
	//���x��������
	vel = { 10, -10 };
	//�n�ʂɂ��Ă��Ȃ�
	isOnGround = false;
}
