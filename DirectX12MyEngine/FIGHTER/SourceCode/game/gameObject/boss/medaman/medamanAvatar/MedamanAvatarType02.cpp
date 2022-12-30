#include "MedamanAvatarType02.h"
#include "Easing.h"

MedamanAvatarType02* MedamanAvatarType02::Create(ObjObject3d* parent, const Vector3& position)
{
	//���_�}�[��(���g�F�^�C�v02)�̃C���X�^���X�𐶐�
	MedamanAvatarType02* medamanAvatarType02 = new MedamanAvatarType02();
	if (medamanAvatarType02 == nullptr) {
		return nullptr;
	}

	//�Q�Ă����Ԃ̃��f�����Z�b�g
	assert(avatarSleepModel);
	medamanAvatarType02->model = avatarSleepModel;

	// ������
	if (!medamanAvatarType02->Initialize()) {
		delete medamanAvatarType02;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	medamanAvatarType02->parent = parent;

	//���W���Z�b�g
	medamanAvatarType02->position = position;

	//�e�q�֌W��ł̊�ʒu���Z�b�g
	medamanAvatarType02->basePos = position;

	return medamanAvatarType02;
}

void MedamanAvatarType02::AttackTypeAvatarBodyBlowLockon()
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

void MedamanAvatarType02::AttackTypeAvatarGatlingMoveCenter()
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

void MedamanAvatarType02::AttackTypeAvatarGiantBulletWait()
{
	//���̃t�F�[�Y��
	attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Move;
}

void MedamanAvatarType02::AttackTypeAvatarGiantBulletMove()
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
