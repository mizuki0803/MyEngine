#include "ComeGoEnemy.h"
#include "Easing.h"

void (ComeGoEnemy::*ComeGoEnemy::actionFuncTable[])() = {
	&ComeGoEnemy::Come,
	&ComeGoEnemy::Attack,
	&ComeGoEnemy::Go
};

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

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();
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
	Vector3 velocity = { 0, 0, 0.1f };
	position += velocity;

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
		isDead = true;
	}
}
