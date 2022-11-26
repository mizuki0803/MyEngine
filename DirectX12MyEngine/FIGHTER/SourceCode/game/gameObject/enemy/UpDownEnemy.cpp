#include "UpDownEnemy.h"
#include "Player.h"
#include "ParticleEmitter.h"

void (UpDownEnemy::* UpDownEnemy::actionFuncTable[])() = {
	&UpDownEnemy::UpBrake,
	&UpDownEnemy::DownBrake,
	&UpDownEnemy::Dead,
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

	//�I�u�W�F�N�g�̍X�V
	Enemy::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();
}

void UpDownEnemy::OnCollision()
{
	//�S�G���ʂ̏Փˏ���
	Enemy::OnCollision();

	//�s�������S�p�ɂ���
	phase = Phase::Dead;

	//���S���ė���]���x�𗐐��ŃZ�b�g
	const float randRotVelX = -1.0f;
	const float randRotBaseVelX = -0.5f;
	crashRotVel.x = (float)rand() / RAND_MAX * randRotVelX + randRotBaseVelX;
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

void UpDownEnemy::Dead()
{
	//��]�����Ȃ���ė�����
	rotation += crashRotVel;

	//�ė����邽�߁A���x�ɉ����x��������
	Vector3 crashAccel = { 0, -0.01f, 0 };
	crashVel += crashAccel;
	//�������鑬�x�̍ő�l��ݒ�
	const float maxCrashSpeed = -0.5f;
	if (crashVel.y <= maxCrashSpeed) { crashVel.y = maxCrashSpeed; }
	position += crashVel;

	//Y���W��0�ȉ��ɂȂ�����폜
	if (position.y <= 0) {
		isDelete = true;

		//�������o�p�p�[�e�B�N������
		ParticleEmitter::GetInstance()->Explosion(position);
	}
}
