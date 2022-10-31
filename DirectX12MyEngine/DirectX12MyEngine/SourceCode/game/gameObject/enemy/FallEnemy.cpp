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

	//�I�u�W�F�N�g�̍X�V
	Enemy::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();	
}

void FallEnemy::OnCollision()
{
	//�S�G���ʂ̏Փˏ���
	Enemy::OnCollision();

	//�s�������S�p�ɂ���
	phase = Phase::Dead;

	//���S���ė���]���x�𗐐��ŃZ�b�g
	const Vector3 randRotVel = { -1.0f, 0.4f, 0.4f };
	const float randRotBaseVelX = -0.25f;
	crashRotVel.x = (float)rand() / RAND_MAX * randRotVel.x + randRotBaseVelX;
	crashRotVel.y = (float)rand() / RAND_MAX * randRotVel.y - randRotVel.y / 2;
	crashRotVel.z = (float)rand() / RAND_MAX * randRotVel.z - randRotVel.z / 2;
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

	//�N���N����]
	const float rotSpeed = 1.0f;
	rotation.y += rotSpeed;
}

void FallEnemy::Dead()
{
	//��]�����Ȃ���ė�����
	rotation += crashRotVel;

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
