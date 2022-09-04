#include "BossAvatarType01.h"
#include "Easing.h"

BossAvatarType01* BossAvatarType01::Create(ObjObject3d* parent, const Vector3& position)
{
	//�{�X(���g�F�^�C�v01)�̃C���X�^���X�𐶐�
	BossAvatarType01* bossAvatarType01 = new BossAvatarType01();
	if (bossAvatarType01 == nullptr) {
		return nullptr;
	}

	//�Q�Ă����Ԃ̃��f�����Z�b�g
	assert(avatarSleepModel);
	bossAvatarType01->model = avatarSleepModel;

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

void BossAvatarType01::AttackTypeAvatarBodyBlowLockon()
{
	//�^�C�}�[���X�V
	const float backTime = 120;
	attackTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= backTime) {
		attackAvatarBodyBlowPhase = AttackTypeAvatarBodyBlowPhase::Shot;

		//��΂����x���m�肳����
		attackAvatarBodyBlowVelocity = attackAvatarBodyBlowLockonPos - GetWorldPos();
		const float moveSpeed = 0.5f;
		attackAvatarBodyBlowVelocity = attackAvatarBodyBlowVelocity.normalize() * moveSpeed;

		//�^�C�}�[������
		attackTimer = 0;
	}
}

void BossAvatarType01::AttackTypeAvatarGatlingMoveCenter()
{
	//�^�C�}�[���X�V
	const float moveTime = 120;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//�e�q�֌W��ł̊�ʒu�Ɉړ�������
	position.x = Easing::OutQuad(basePos.x, attackAvatarGatlingLength, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= moveTime) {
		attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::RotStart;

		//��]�J�n�p�Ɋp�x��ݒ肷��
		attackAvatarGatlingAngle = 0;

		//�^�C�}�[������
		attackTimer = 0;
	}
}

void BossAvatarType01::AttackTypeAvatarGiantBulletWait()
{
	//�^�C�}�[���X�V
	const float waitTime = 90;
	attackTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= waitTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Move;

		//�^�C�}�[������
		attackTimer = 0;
	}
}

void BossAvatarType01::AttackTypeAvatarGiantBulletMove()
{
	//�^�C�}�[���X�V
	const float moveTime = 150;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//���ˈʒu�Ɉړ�������
	const Vector3 movePos = { 6, 0, 0 };
	position.x = Easing::OutQuad(basePos.x, movePos.x, time);
	position.y = Easing::OutQuad(basePos.y, movePos.y, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= moveTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::ChargeShot;

		//�^�C�}�[������
		attackTimer = 0;
	}
}
