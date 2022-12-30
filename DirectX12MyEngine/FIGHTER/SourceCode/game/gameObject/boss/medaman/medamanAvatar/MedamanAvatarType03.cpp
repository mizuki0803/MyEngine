#include "MedamanAvatarType03.h"
#include "Easing.h"
#include "ParticleEmitter.h"

MedamanAvatarType03* MedamanAvatarType03::Create(ObjObject3d* parent, const Vector3& position)
{
	//���_�}�[��(���g�F�^�C�v03)�̃C���X�^���X�𐶐�
	MedamanAvatarType03* medamanAvatarType03 = new MedamanAvatarType03();
	if (medamanAvatarType03 == nullptr) {
		return nullptr;
	}

	//�Q�Ă����Ԃ̃��f�����Z�b�g
	assert(avatarSleepModel);
	medamanAvatarType03->model = avatarSleepModel;

	// ������
	if (!medamanAvatarType03->Initialize()) {
		delete medamanAvatarType03;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	medamanAvatarType03->parent = parent;

	//���W���Z�b�g
	medamanAvatarType03->position = position;

	//�e�q�֌W��ł̊�ʒu���Z�b�g
	medamanAvatarType03->basePos = position;

	return medamanAvatarType03;
}

bool MedamanAvatarType03::Initialize()
{
	//���ʂ̎��S���̓���������ƃ��_�}�[���{�̂ɔ킳���Ă��܂����߁AZ�����ɂ�������
	crashVel.z = 1.0f;

	//���_�}�[�����g���ʏ�����
	if (!MedamanAvatar::Initialize()) {
		return false;
	}

	return true;
}

void MedamanAvatarType03::AttackTypeAvatarBodyBlowLockon()
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

void MedamanAvatarType03::AttackTypeAvatarGatlingMoveCenter()
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

void MedamanAvatarType03::AttackTypeAvatarGiantBulletWait()
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

void MedamanAvatarType03::AttackTypeAvatarGiantBulletMove()
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

void MedamanAvatarType03::DeadAction()
{
	//X����]�����Ȃ���ė�����
	const float rotSpeed = 1.0f;
	rotation.x += rotSpeed;

	//���ʂ̎��S���̓���������ƃ��_�}�[���{�̂ɔ킳���Ă��܂����߁AZ�����ɂ�������
	//�ė����邽�߁A���x�ɉ����x��������
	Vector3 crashAccel = { 0, -0.005f, -0.04f };
	crashVel += crashAccel;
	//�������鑬�x�̍ő�l��ݒ�
	const float maxCrashSpeed = -0.4f;
	if (crashVel.y <= maxCrashSpeed) { crashVel.y = maxCrashSpeed; }
	if (crashVel.z <= 0) { crashVel.z = 0; }
	position += crashVel;

	//�����p�[�e�B�N������
	DeadBlackSmoke();

	//Y���W��0�ȉ��ɂȂ�����폜
	if (GetWorldPos().y <= 0) {
		isDelete = true;

		//�������o�p�p�[�e�B�N������
		ParticleEmitter::GetInstance()->Explosion(GetWorldPos());
	}
}
