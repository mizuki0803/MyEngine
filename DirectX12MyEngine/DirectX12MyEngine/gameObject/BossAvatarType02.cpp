#include "BossAvatarType02.h"
#include "Easing.h"

BossAvatarType02* BossAvatarType02::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//�{�X(���g�F�^�C�v02)�̃C���X�^���X�𐶐�
	BossAvatarType02* bossAvatarType02 = new BossAvatarType02();
	if (bossAvatarType02 == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossAvatarType02->model = model;

	// ������
	if (!bossAvatarType02->Initialize()) {
		delete bossAvatarType02;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	bossAvatarType02->parent = parent;

	//���W���Z�b�g
	bossAvatarType02->position = position;

	//�e�q�֌W��ł̊�ʒu���Z�b�g
	bossAvatarType02->basePos = position;

	return bossAvatarType02;
}

void BossAvatarType02::AttackTypeBLockon()
{
	//�^�C�}�[���X�V
	const float backTime = 150;
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

void BossAvatarType02::AttackTypeCMoveCenter()
{
	//�^�C�}�[���X�V
	const float moveTime = 120;
	attackCTimer++;
	const float time = attackCTimer / moveTime;

	//�e�q�֌W��ł̊�ʒu�Ɉړ�������
	position.x = Easing::OutQuad(basePos.x, -attackCLength, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackCTimer >= moveTime) {
		attackCPhase = AttackTypeCPhase::RotStart;

		//��]�J�n�p�Ɋp�x��ݒ肷��
		attackCAngle = 180;

		//�^�C�}�[������
		attackCTimer = 0;
	}
}

void BossAvatarType02::AttackTypeDWait()
{
	//���̃t�F�[�Y��
	attackDPhase = AttackTypeDPhase::Move;
}

void BossAvatarType02::AttackTypeDMove()
{
	//�^�C�}�[���X�V
	const float moveTime = 150;
	attackDTimer++;
	const float time = attackDTimer / moveTime;

	//���ˈʒu�Ɉړ�������
	const Vector3 movePos = { -21, 0, 0 };
	position.x = Easing::OutQuad(basePos.x, movePos.x, time);
	position.y = Easing::OutQuad(basePos.y, movePos.y, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackDTimer >= moveTime) {
		attackDPhase = AttackTypeDPhase::ChargeShot;

		//�^�C�}�[������
		attackDTimer = 0;
	}
}
