#include "BossAvatarType03.h"
#include "Easing.h"

BossAvatarType03* BossAvatarType03::Create(ObjObject3d* parent, const Vector3& position)
{
	//�{�X(���g�F�^�C�v03)�̃C���X�^���X�𐶐�
	BossAvatarType03* bossAvatarType03 = new BossAvatarType03();
	if (bossAvatarType03 == nullptr) {
		return nullptr;
	}

	//�Q�Ă����Ԃ̃��f�����Z�b�g
	assert(avatarSleepModel);
	bossAvatarType03->model = avatarSleepModel;

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

void BossAvatarType03::AttackTypeAvatarBodyBlowLockon()
{
	//�^�C�}�[���X�V
	const float backTime = 210;
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

void BossAvatarType03::AttackTypeAvatarGatlingMoveCenter()
{
	//�^�C�}�[���X�V
	const float moveTime = 120;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//�e�q�֌W��ł̊�ʒu�Ɉړ�������
	position.y = Easing::OutQuad(basePos.y, attackAvatarGatlingLength, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= moveTime) {
		attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::RotStart;

		//��]�J�n�p�Ɋp�x��ݒ肷��
		attackAvatarGatlingAngle = 90;

		//�^�C�}�[������
		attackTimer = 0;
	}
}

void BossAvatarType03::AttackTypeAvatarGiantBulletWait()
{
	//�^�C�}�[���X�V
	const float waitTime = 30;
	attackTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= waitTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Move;

		//�^�C�}�[������
		attackTimer = 0;
	}
}

void BossAvatarType03::AttackTypeAvatarGiantBulletMove()
{
	//�^�C�}�[���X�V
	const float moveTime = 150;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//���ˈʒu�Ɉړ�������
	const Vector3 movePos = { -2, 0, 0 };
	position.x = Easing::OutQuad(basePos.x, movePos.x, time);
	position.y = Easing::OutQuad(basePos.y, movePos.y, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= moveTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::ChargeShot;

		//�^�C�}�[������
		attackTimer = 0;
	}
}
