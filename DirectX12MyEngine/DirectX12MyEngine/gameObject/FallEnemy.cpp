#include "FallEnemy.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (FallEnemy::*FallEnemy::actionFuncTable[])() = {
	&FallEnemy::Fall,
	&FallEnemy::Attack,
	&FallEnemy::Dead,
};

FallEnemy* FallEnemy::Create(ObjModel* model, const Vector3& position)
{
	//�G�̃C���X�^���X�𐶐�
	FallEnemy* upDownEnemy = new FallEnemy();
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

	//�������W���Z�b�g
	upDownEnemy->startPos = position;

	//���W���Z�b�g
	upDownEnemy->position = position;

	return upDownEnemy;
}

void FallEnemy::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();	
}

void FallEnemy::OnCollision()
{
	//�S�G���ʂ̏Փˏ���
	Enemy::OnCollision();

	//�s�������S�p�ɂ���
	phase = Phase::Dead;
}

void FallEnemy::Fall()
{
	//�C�[�W���O�ō~������
	const float fallTime = 60;
	fallTimer++;
	const float time = fallTimer / fallTime;

	//�����ʒu����100�~�肽�Ƃ���Œ�~����
	const float stayPosY = startPos.y - 100;
	position.y = Easing::OutQuart(startPos.y, stayPosY, time);

	//�C�[�W���O���I�������玟�̃t�F�[�Y��
	if (fallTimer >= fallTime) {
		phase = Phase::Attack;
	}
}

void FallEnemy::Attack()
{
	//���˃^�C�}�[�J�E���g�_�E��
	--fireTimer;
	if (fireTimer <= 0) {
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		fireTimer = fireInterval;
	}
}

void FallEnemy::Dead()
{
	//X����]�����Ȃ���ė�����
	const float rotSpeed = 0.5f;
	rotation.x -= rotSpeed;

	//�ė����邽�߁A���x�ɉ����x��������
	Vector3 crashAccel = { 0, -0.005f, 0 };
	crashVel += crashAccel;
	//�������鑬�x�̍ő�l��ݒ�
	const float maxCrashSpeed = -0.2f;
	if (crashVel.y <= maxCrashSpeed) { crashVel.y = maxCrashSpeed; }
	position += crashVel;

	//Y���W��0�ȉ��ɂȂ�����폜
	if (position.y <= 0) {
		isDelete = true;

		//�������o�p�p�[�e�B�N������
		ParticleEmitter::GetInstance()->Explosion(GetWorldPos());
	}
}
