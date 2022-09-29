#include "CannonEnemy.h"
#include "Player.h"

void (CannonEnemy::* CannonEnemy::actionFuncTable[])() = {
	&CannonEnemy::Attack,
	&CannonEnemy::Dead,
};

ObjModel* CannonEnemy::breakModel = nullptr;

CannonEnemy* CannonEnemy::Create(ObjModel* model, const Vector3& position)
{
	//�G�̃C���X�^���X�𐶐�
	CannonEnemy* cannon = new CannonEnemy();
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

	return cannon;
}

void CannonEnemy::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();
}

void CannonEnemy::OnCollision()
{
	//���S���Ă��폜���Ȃ����f���ύX�̂��߁A���S���Ă����甲����
	if (isDead) { return; }

	//�S�G���ʂ̏Փˏ���
	Enemy::OnCollision();

	//�s�������S�p�ɂ���
	phase = Phase::Dead;

	//�j�󂳂ꂽ���f���ɕύX
	model = breakModel;
}

void CannonEnemy::Attack()
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

void CannonEnemy::Dead()
{
}
