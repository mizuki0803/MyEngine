#include "BossAvatarType02.h"
#include "Easing.h"

BossAvatarType02* BossAvatarType02::Create(ObjObject3d* parent, const Vector3& position)
{
	//�{�X(���g�F�^�C�v02)�̃C���X�^���X�𐶐�
	BossAvatarType02* bossAvatarType02 = new BossAvatarType02();
	if (bossAvatarType02 == nullptr) {
		return nullptr;
	}

	//�Q�Ă����Ԃ̃��f�����Z�b�g
	assert(avatarSleepModel);
	bossAvatarType02->model = avatarSleepModel;

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

void BossAvatarType02::AttackTypeAvatarBodyBlowLockon()
{
	//�^�C�}�[���X�V
	const float backTime = 150;
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

void BossAvatarType02::AttackTypeAvatarGatlingMoveCenter()
{
	//�^�C�}�[���X�V
	const float moveTime = 120;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//�e�q�֌W��ł̊�ʒu�Ɉړ�������
	position.x = Easing::OutQuad(basePos.x, -attackAvatarGatlingLength, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= moveTime) {
		attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::RotStart;

		//��]�J�n�p�Ɋp�x��ݒ肷��
		attackAvatarGatlingAngle = 180;

		//�^�C�}�[������
		attackTimer = 0;
	}
}

void BossAvatarType02::AttackTypeAvatarGiantBulletWait()
{
	//���̃t�F�[�Y��
	attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Move;
}

void BossAvatarType02::AttackTypeAvatarGiantBulletMove()
{
	//�^�C�}�[���X�V
	const float moveTime = 150;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//���ˈʒu�Ɉړ�������
	const Vector3 movePos = { -6, 0, 0 };
	position.x = Easing::OutQuad(basePos.x, movePos.x, time);
	position.y = Easing::OutQuad(basePos.y, movePos.y, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= moveTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::ChargeShot;

		//�^�C�}�[������
		attackTimer = 0;
	}
}
