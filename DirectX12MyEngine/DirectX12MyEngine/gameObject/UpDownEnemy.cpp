#include "UpDownEnemy.h"
#include "Player.h"

void (UpDownEnemy::* UpDownEnemy::actionFuncTable[])() = {
	&UpDownEnemy::UpBrake,
	&UpDownEnemy::DownBrake,
};

UpDownEnemy* UpDownEnemy::Create(ObjModel* model, const Vector3& position)
{
	//�G�̃C���X�^���X�𐶐�
	UpDownEnemy* upDownEnemy = new UpDownEnemy();
	if (upDownEnemy == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	upDownEnemy->model = model;

	// ������
	if (!upDownEnemy->Initialize()) {
		delete upDownEnemy;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	upDownEnemy->position = position;

	return upDownEnemy;
}

void UpDownEnemy::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();
}

void UpDownEnemy::UpBrake()
{
	//�㏸��}����
	Vector3 accel = { 0, -0.01f, 0 };
	velocity += accel;
	position += velocity;


	//�w�肵�����x�܂ŉ��~��ԂɂȂ����牺�~�Ƀu���[�L��������
	const float changePhaseVelY = -0.5f;
	if (velocity.y <= changePhaseVelY) {
		phase = Phase::DownBrake;
	}
}

void UpDownEnemy::DownBrake()
{
	//���~��}����
	Vector3 accel = { 0, 0.01f, 0 };
	velocity += accel;
	position += velocity;


	//�w�肵�����x�܂ŏ㏸��ԂɂȂ�����㏸�Ƀu���[�L��������
	const float changePhaseVelY = 0.5f;
	if (velocity.y >= changePhaseVelY) {
		phase = Phase::UpBrake;
	}
}
