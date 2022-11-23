#include "ComeGoEnemy.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (ComeGoEnemy::*ComeGoEnemy::actionFuncTable[])() = {
	&ComeGoEnemy::Come,
	&ComeGoEnemy::Attack,
	&ComeGoEnemy::Go,
	&ComeGoEnemy::Dead,
};

float ComeGoEnemy::attackMoveSpeed = 0;

ComeGoEnemy* ComeGoEnemy::Create(ObjModel* model, const Vector3& startPos, const Vector3& comePos, const Vector3& goTargetPos, const int attackTime)
{
	//�G�̃C���X�^���X�𐶐�
	ComeGoEnemy* comeGoEnemy = new ComeGoEnemy();
	if (comeGoEnemy == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	comeGoEnemy->model = model;

	// ������
	if (!comeGoEnemy->Initialize()) {
		delete comeGoEnemy;
		assert(0);
		return nullptr;
	}

	//�������W���Z�b�g
	comeGoEnemy->startPos = startPos;

	//�������W���Z�b�g
	comeGoEnemy->comePos = comePos;

	//�o���ڕW���W���Z�b�g
	comeGoEnemy->goTargetPos = goTargetPos;

	//�U�����Ԃ��Z�b�g
	comeGoEnemy->attackTime = attackTime;

	return comeGoEnemy;
}

void ComeGoEnemy::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//�I�u�W�F�N�g�̍X�V
	Enemy::Update();
}

void ComeGoEnemy::OnCollision()
{
	//�S�G���ʂ̏Փˏ���
	Enemy::OnCollision();

	//�s�������S�p�ɂ���
	phase = Phase::Dead;

	//���S���ė���]���x�𗐐��ŃZ�b�g
	const Vector3 randRotVel = { 0, 0.4f, 4.0f };
	crashRotVel.x = (float)rand() / RAND_MAX * randRotVel.x - randRotVel.y / 2;
	crashRotVel.y = (float)rand() / RAND_MAX * randRotVel.y - randRotVel.y / 2;
	crashRotVel.z = (float)rand() / RAND_MAX * randRotVel.z - randRotVel.z / 2;
}

void ComeGoEnemy::Come()
{
	//�C�[�W���O�œ���������
	const float comeTime = 120;
	comeTimer++;
	const float time = comeTimer / comeTime;
	position.x = Easing::OutQuart(startPos.x, comePos.x, time);
	position.y = Easing::OutQuart(startPos.y, comePos.y, time);
	position.z = Easing::OutQuart(startPos.z, comePos.z, time);

	//�C�[�W���O���I�������玟�̃t�F�[�Y��
	if (comeTimer >= comeTime) {
		phase = Phase::Attack;
	}
}

void ComeGoEnemy::Attack()
{
	//�U�����鎞�Ԃ��J�E���g
	attackTimer++;

	//Z�����Ɉړ�������
	position.z += attackMoveSpeed;

	//���˃^�C�}�[�J�E���g�_�E��
	--fireTimer;
	if (fireTimer <= 0) {
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		fireTimer = fireInterval;
	}

	//���ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= attackTime) {
		phase = Phase::Go;

		//�o���J�n���W���Z�b�g
		goStartPos = position;
	}
}

void ComeGoEnemy::Go()
{
	//�C�[�W���O�ŏo��������
	const float goTime = 120;
	goTimer++;
	const float time = goTimer / goTime;
	position.x = Easing::InQuart(goStartPos.x, goTargetPos.x, time);
	position.y = Easing::InQuart(goStartPos.y, goTargetPos.y, time);
	position.z = Easing::InQuart(goStartPos.z, goTargetPos.z, time);

	//�C�[�W���O���I��������폜����
	if (goTimer >= goTime) {
		isDelete = true;
	}
}

void ComeGoEnemy::Dead()
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