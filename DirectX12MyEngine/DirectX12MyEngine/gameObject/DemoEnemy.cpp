#include "DemoEnemy.h"

DemoEnemy* DemoEnemy::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
{
	//�G�̃C���X�^���X�𐶐�
	DemoEnemy* demoEnemy = new DemoEnemy();
	if (demoEnemy == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	demoEnemy->model = model;

	// ������
	if (!demoEnemy->Initialize()) {
		delete demoEnemy;
		assert(0);
		return nullptr;
	}


	//���W���Z�b�g
	demoEnemy->position = position;

	//���x���Z�b�g
	demoEnemy->velocity = velocity;

	return demoEnemy;
}

bool DemoEnemy::Initialize()
{
	//3D�I�u�W�F�N�g�̏�����
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	PreviousPhaseInit();

	return true;
}

void DemoEnemy::Update()
{
	//�ړ�
	Move();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();
}

//void DemoEnemy::OnCollision()
//{
//}

void DemoEnemy::PreviousPhaseInit()
{
	//���˃^�C�}�[��������
	fireTimer = fireInterval;
}

void DemoEnemy::Move()
{
	switch (phase)
	{
	case Phase::Previous:
	default:
		//�O�i����
		position -= velocity;

		////���̈ʒu�܂őO�i��������
		//if (position.z < 0.0f) {
		//	phase = Phase::Back;
		//}

		////���˃^�C�}�[�J�E���g�_�E��
		//--fireTimer;
		//if (fireTimer <= 0) {
		//	//�e�𔭎�
		//	Fire();
		//	//���˃^�C�}�[��������
		//	fireTimer = fireInterval;
		//}

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
