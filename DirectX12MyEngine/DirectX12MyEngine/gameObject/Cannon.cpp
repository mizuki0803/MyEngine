#include "Cannon.h"

Cannon* Cannon::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
{
	//�G�̃C���X�^���X�𐶐�
	Cannon* cannon = new Cannon();
	if (cannon == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	cannon->model = model;

	// ������
	if (!cannon->Initialize()) {
		delete cannon;
		assert(0);
		return nullptr;
	}


	//���W���Z�b�g
	cannon->position = position;

	//���x���Z�b�g
	cannon->velocity = velocity;

	return cannon;
}

bool Cannon::Initialize()
{
	//3D�I�u�W�F�N�g�̏�����
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	PreviousPhaseInit();

	return true;
}

void Cannon::Update()
{
	//�ړ�
	Move();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();
}

//void Cannon::OnCollision()
//{
//}

void Cannon::PreviousPhaseInit()
{
	//���˃^�C�}�[��������
	fireTimer = fireInterval;
}

void Cannon::Move()
{
	switch (phase)
	{
	case Phase::Previous:
	default:
		//�O�i����
		position -= velocity;

		//���̈ʒu�܂őO�i��������
		if (position.z < 0.0f) {
			phase = Phase::Back;
		}

		//���˃^�C�}�[�J�E���g�_�E��
		--fireTimer;
		if (fireTimer <= 0) {
			//�e�𔭎�
			Fire();
			//���˃^�C�}�[��������
			fireTimer = fireInterval;
		}

		break;
	case Phase::Back:
		//��ނ���
		position += velocity;
		//���̈ʒu�܂Ō�ނ�����O�i
		if (position.z > 50.0f) {
			phase = Phase::Previous;
		}

		break;
	}
}
