#include "BossAvatarType03.h"
#include "Easing.h"

BossAvatarType03* BossAvatarType03::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//�{�X(���g�F�^�C�v03)�̃C���X�^���X�𐶐�
	BossAvatarType03* bossAvatarType03 = new BossAvatarType03();
	if (bossAvatarType03 == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossAvatarType03->model = model;

	// ������
	if (!bossAvatarType03->Initialize()) {
		delete bossAvatarType03;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	bossAvatarType03->parent = parent;

	//���W���Z�b�g
	bossAvatarType03->position = position;

	//�e�q�֌W��ł̊�ʒu���Z�b�g
	bossAvatarType03->basePos = position;

	return bossAvatarType03;
}

void BossAvatarType03::AttackTypeBLockon()
{
	//�^�C�}�[���X�V
	const float backTime = 210;
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

void BossAvatarType03::AttackTypeCMoveCenter()
{
	//�^�C�}�[���X�V
	const float moveTime = 120;
	attackCTimer++;
	const float time = attackCTimer / moveTime;

	//�e�q�֌W��ł̊�ʒu�Ɉړ�������
	position.y = Easing::OutQuad(basePos.y, attackCLength, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackCTimer >= moveTime) {
		attackCPhase = AttackTypeCPhase::RotStart;

		//��]�J�n�p�Ɋp�x��ݒ肷��
		attackCAngle = 90;

		//�^�C�}�[������
		attackCTimer = 0;
	}
}

void BossAvatarType03::AttackTypeDWait()
{
	//�^�C�}�[���X�V
	const float waitTime = 30;
	attackDTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackDTimer >= waitTime) {
		attackDPhase = AttackTypeDPhase::Move;

		//�^�C�}�[������
		attackDTimer = 0;
	}
}

void BossAvatarType03::AttackTypeDMove()
{
	//�^�C�}�[���X�V
	const float moveTime = 150;
	attackDTimer++;
	const float time = attackDTimer / moveTime;

	//���ˈʒu�Ɉړ�������
	const Vector3 movePos = { -7, 0, 0 };
	position.x = Easing::OutQuad(basePos.x, movePos.x, time);
	position.y = Easing::OutQuad(basePos.y, movePos.y, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackDTimer >= moveTime) {
		attackDPhase = AttackTypeDPhase::ChargeShot;

		//�^�C�}�[������
		attackDTimer = 0;
	}
}
