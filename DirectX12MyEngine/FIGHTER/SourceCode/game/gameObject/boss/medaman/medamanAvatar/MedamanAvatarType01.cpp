#include "MedamanAvatarType01.h"
#include "Easing.h"

MedamanAvatarType01* MedamanAvatarType01::Create(ObjObject3d* parent, const Vector3& position)
{
	//���_�}�[��(���g�F�^�C�v01)�̃C���X�^���X�𐶐�
	MedamanAvatarType01* medamanAvatarType01 = new MedamanAvatarType01();
	if (medamanAvatarType01 == nullptr) {
		return nullptr;
	}

	//�Q�Ă����Ԃ̃��f�����Z�b�g
	assert(avatarSleepModel);
	medamanAvatarType01->model = avatarSleepModel;

	// ������
	if (!medamanAvatarType01->Initialize()) {
		delete medamanAvatarType01;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	medamanAvatarType01->parent = parent;

	//���W���Z�b�g
	medamanAvatarType01->position = position;

	//�e�q�֌W��ł̊�ʒu���Z�b�g
	medamanAvatarType01->basePos = position;

	return medamanAvatarType01;
}

void MedamanAvatarType01::AttackTypeAvatarBodyBlowLockon()
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

void MedamanAvatarType01::AttackTypeAvatarGatlingMoveCenter()
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

void MedamanAvatarType01::AttackTypeAvatarGiantBulletWait()
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

void MedamanAvatarType01::AttackTypeAvatarGiantBulletMove()
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
