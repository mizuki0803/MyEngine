#include "BossAvatarType04.h"
#include "Easing.h"

BossAvatarType04* BossAvatarType04::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//�{�X(���g�F�^�C�v04)�̃C���X�^���X�𐶐�
	BossAvatarType04* bossAvatarType04 = new BossAvatarType04();
	if (bossAvatarType04 == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossAvatarType04->model = model;

	// ������
	if (!bossAvatarType04->Initialize()) {
		delete bossAvatarType04;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	bossAvatarType04->parent = parent;

	//���W���Z�b�g
	bossAvatarType04->position = position;

	//�e�q�֌W��ł̒�ʒu���Z�b�g
	bossAvatarType04->fixedPos = position;

	return bossAvatarType04;
}

void BossAvatarType04::AttackTypeBLockon()
{
	//�^�C�}�[���X�V
	const float backTime = 180;
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

void BossAvatarType04::AttackTypeCMoveCenter()
{
	//�^�C�}�[���X�V
	const float moveTime = 120;
	attackCTimer++;
	const float time = attackCTimer / moveTime;

	//���S�Ɉړ�������
	position.y = Easing::OutQuad(fixedPos.y, -attackCLength, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackCTimer >= moveTime) {
		attackCPhase = AttackTypeCPhase::RotStart;

		//��]�J�n�p�Ɋp�x��ݒ肷��
		attackCAngle = 270;

		//�^�C�}�[������
		attackCTimer = 0;
	}
}

void BossAvatarType04::AttackTypeDWait()
{
	//�^�C�}�[���X�V
	const float waitTime = 60;
	attackDTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackDTimer >= waitTime) {
		attackDPhase = AttackTypeDPhase::Move;

		//�^�C�}�[������
		attackDTimer = 0;
	}
}

void BossAvatarType04::AttackTypeDMove()
{
	//�^�C�}�[���X�V
	const float moveTime = 150;
	attackDTimer++;
	const float time = attackDTimer / moveTime;

	//���ˈʒu�Ɉړ�������
	const Vector3 movePos = { 7, 0, 0 };
	position.x = Easing::OutQuad(fixedPos.x, movePos.x, time);
	position.y = Easing::OutQuad(fixedPos.y, movePos.y, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackDTimer >= moveTime) {
		attackDPhase = AttackTypeDPhase::ChargeShot;

		//�^�C�}�[������
		attackDTimer = 0;
	}
}
