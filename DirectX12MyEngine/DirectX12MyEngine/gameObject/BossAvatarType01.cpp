#include "BossAvatarType01.h"
#include "Easing.h"

BossAvatarType01* BossAvatarType01::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//�{�X(���g�F�^�C�v01)�̃C���X�^���X�𐶐�
	BossAvatarType01* bossAvatarType01 = new BossAvatarType01();
	if (bossAvatarType01 == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossAvatarType01->model = model;

	// ������
	if (!bossAvatarType01->Initialize()) {
		delete bossAvatarType01;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	bossAvatarType01->parent = parent;

	//���W���Z�b�g
	bossAvatarType01->position = position;

	//�e�q�֌W��ł̊�ʒu���Z�b�g
	bossAvatarType01->basePos = position;

	return bossAvatarType01;
}

void BossAvatarType01::AttackTypeBLockon()
{
	//�^�C�}�[���X�V
	const float backTime = 120;
	attackBTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackBTimer >= backTime) {
		attackBPhase = AttackTypeBPhase::Shot;

		//��΂����x���m�肳����
		attackBVelocity = attackBLockonPos - GetWorldPos();
		const float moveSpeed = 1.0f;
		attackBVelocity = attackBVelocity.normalize() * moveSpeed;

		//�^�C�}�[������
		attackBTimer = 0;
	}
}

void BossAvatarType01::AttackTypeCMoveCenter()
{
	//�^�C�}�[���X�V
	const float moveTime = 120;
	attackCTimer++;
	const float time = attackCTimer / moveTime;

	//�e�q�֌W��ł̊�ʒu�Ɉړ�������
	position.x = Easing::OutQuad(basePos.x, attackCLength, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackCTimer >= moveTime) {
		attackCPhase = AttackTypeCPhase::RotStart;

		//��]�J�n�p�Ɋp�x��ݒ肷��
		attackCAngle = 0;

		//�^�C�}�[������
		attackCTimer = 0;
	}
}

void BossAvatarType01::AttackTypeDWait()
{
	//�^�C�}�[���X�V
	const float waitTime = 90;
	attackDTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackDTimer >= waitTime) {
		attackDPhase = AttackTypeDPhase::Move;

		//�^�C�}�[������
		attackDTimer = 0;
	}
}

void BossAvatarType01::AttackTypeDMove()
{
	//�^�C�}�[���X�V
	const float moveTime = 150;
	attackDTimer++;
	const float time = attackDTimer / moveTime;

	//���ˈʒu�Ɉړ�������
	const Vector3 movePos = { 21, 0, 0 };
	position.x = Easing::OutQuad(basePos.x, movePos.x, time);
	position.y = Easing::OutQuad(basePos.y, movePos.y, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackDTimer >= moveTime) {
		attackDPhase = AttackTypeDPhase::ChargeShot;

		//�^�C�}�[������
		attackDTimer = 0;
	}
}
