#include "DemoEnemy.h"

void (DemoEnemy::* DemoEnemy::actionFuncTable[])() = {
	&DemoEnemy::Previous,
	&DemoEnemy::Back
};

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

	return true;
}

void DemoEnemy::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();
}

void DemoEnemy::Previous()
{
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
}

void DemoEnemy::Back()
{
	//��ނ���
	position += velocity;
	//���̈ʒu�܂Ō�ނ�����O�i
	if (position.z > 50.0f) {
		phase = Phase::Previous;
	}
}
