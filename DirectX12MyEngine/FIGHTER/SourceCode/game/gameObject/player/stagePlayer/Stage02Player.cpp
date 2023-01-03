#include "Stage02Player.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (Stage02Player::* Stage02Player::stageClearActionFuncTable[])() = {
	&Stage02Player::StageClearStay,
	&Stage02Player::StageClearAdvance,
	&Stage02Player::StageClearBoost,
};

Stage02Player* Stage02Player::Create(ObjModel* model, const int startHP, const int maxHP)
{
	//�X�e�[�W02���@�̃C���X�^���X�𐶐�
	Stage02Player* player = new Stage02Player();
	if (player == nullptr) {
		return nullptr;
	}

	// ������
	if (!player->Initialize(model, startHP, maxHP)) {
		delete player;
		assert(0);
		return nullptr;
	}

	return player;
}

bool Stage02Player::Initialize(ObjModel* model, const int startHP, const int maxHP)
{
	//��̍��W���Z�b�g
	basePos = { 0, -2, 16 };

	//���@�̋��ʏ�����
	if (!BasePlayer::Initialize(model, startHP, maxHP)) {
		return false;
	}

	//���@�̈ړ����E���Z�b�g
	moveLimitMin = { -15.0f, -8.0f };
	moveLimitMax = { 15.0f, 8.0f };

	return true;
}

void Stage02Player::StageClearModeStart()
{
	//�J�����Ǐ]������
	SetIsCameraFollow(false);
	//�J�����Ǐ]�����ɂ��A���[�J�����W�Ƀ��[���h���W����
	position = GetWorldPos();

	//�p�[�e�B�N���̑傫���𓝈ꂷ�邽�߁A�ړ��͂������Ȃ����ʏ�ړ���Ԃɂ��Ă���
	moveSpeedPhase = MoveSpeedPhase::NormalSpeed;

	//�X�e�[�W�N���A��̓���������
	isStageClearMode = true;
}

void Stage02Player::CrashStart()
{
	//�����������ɏC�����Ă���
	const float crashStartRota = 10;
	rotation.x = crashStartRota;

	//�S�G���ʂ̒ė��J�n
	BasePlayer::CrashStart();
}

void Stage02Player::Crash()
{
	//�ė���]����
	const Vector3 rotSpeed = { 0.25f, 0, 4 + crashTimer * 0.01f };
	rotation += rotSpeed;
	const float crashRotLimit = 90;
	rotation.x = min(rotation.x, crashRotLimit);

	//���@���X���Ă���p�x�Ɉړ�������
	const float crashMoveSpeed = 0.5f;
	Vector3 velocity = { moveBaseSpeed, 1.0f + crashTimer * 0.01f, crashMoveSpeed };
	velocity.x *= (rotation.y / rotLimit.y);
	velocity.y *= -(rotation.x / crashRotLimit);
	velocity.z *= (1 - (rotation.x / crashRotLimit));
	//�O�ɐi�ޑ��x��0�ɂȂ�Ȃ��悤�ɂ��Ă������߁A�ŏ��l��ݒ�
	const Vector3 velLimit = { 0, -1.5f, 0.1f };
	velocity.y = max(velocity.y, velLimit.y);
	velocity.z = max(velocity.z, velLimit.z);
	position += velocity;

	//�u���[�̋�����ʏ�ړ����̋����ɖ߂��Ă���
	SpeedChangeNormalBlur();

	//�ė��^�C�}�[�X�V
	crashTimer++;
	//�ė���Ԃ̍ő厞�Ԃ��z���Ă��Ȃ���Δ�����
	const int crashTimeMax = 240;
	if (crashTimer < crashTimeMax) { return; }

	//���S
	isDead = true;

	//�������o�p�p�[�e�B�N������
	const float explosionSize = 2.5f;
	const int explosionTime = 60;
	ParticleEmitter::GetInstance()->Explosion(position, explosionSize, explosionTime);
}

void Stage02Player::StageClear()
{
	//�X�e�[�W�N���A��s��
	(this->*stageClearActionFuncTable[static_cast<size_t>(stageClearModePhase)])();
}

void Stage02Player::StageClearStay()
{
	//�p�x��0���Ă���
	StageClearRotateFix();

	//���@���X���Ă���p�x�Ɉړ�������
	Vector3 velocity = { 0, 0, 0 };
	velocity.x = moveBaseSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveBaseSpeed * -(rotation.x / rotLimit.x);
	position += velocity;

	//�u���[�̋�����ʏ�ړ����̋����ɖ߂��Ă���
	SpeedChangeNormalBlur();
}

void Stage02Player::StageClearRotateFix()
{
	//��]���x
	const float rotSpeed = 1.0f;
	//�p�x�C������x
	const float backBaseSpeed = rotSpeed / 1.8f;
	Vector3 rot = { 0, 0, 0 };

	//y����]
	{
		//�p�x�C�����x�{��
		float backSpeedRatio = fabsf(rotation.y / (rotLimit.y * 2)) + 0.5f;
		//�p�x�C�����x
		const float backSpeed = backBaseSpeed * backSpeedRatio;
		//y����]�̌X�����C������
		const float rotMin = 0.5f;
		if (rotation.y > rotMin) {
			rot.y -= backSpeed;
		}
		else if (rotation.y < -rotMin) {
			rot.y += backSpeed;
		}
		else {
			rotation.y = 0;
		}
	}
	//x����]
	{
		//�p�x�C�����x�{��
		float backSpeedRatio = fabsf(rotation.x / (rotLimit.x * 2)) + 0.5f;
		//�p�x�C�����x
		const float backSpeed = backBaseSpeed * backSpeedRatio;
		//x����]�̌X�����C������
		const float rotMin = 0.5f;
		if (rotation.x > rotMin) {
			rot.x -= backSpeed;
		}
		else if (rotation.x < -rotMin) {
			rot.x += backSpeed;
		}
		else {
			rotation.x = 0;
		}
	}
	//z����]
	{
		//�������Ȃ��Ǝ₵���̂ł���炳���Ă���
		const float rotZSpeed = 0.04f;
		const float rotZLimit = 1.0f;
		//�E��]
		if (isRotZRight) {
			swayZ += rotZSpeed;
			if (swayZ >= rotZLimit) {
				isRotZRight = false;
			}
		}
		//����]
		else {
			swayZ -= rotZSpeed;
			if (swayZ <= -rotZLimit) {
				isRotZRight = true;
			}
		}

		rotation.z = -rotation.y + swayZ;
	}

	//��]�̍X�V
	rotation += rot;

	//�p�x�̌��E�l����͂ݏo���Ȃ�
	rotation.y = max(rotation.y, -rotLimit.y);
	rotation.y = min(rotation.y, +rotLimit.y);
	rotation.x = max(rotation.x, -rotLimit.x);
	rotation.x = min(rotation.x, +rotLimit.x);
}

void Stage02Player::StageClearAdvance()
{
	//���@���X���Ă���p�x�ɑO�i������
	Vector3 velocity = { 0, 0, 0.2f };
	velocity = MatrixTransformDirection(velocity, matWorld);
	position += velocity;
}

void Stage02Player::StageClearBoost()
{
	//�u�[�X�g�ړ�
	//���@���X���Ă���p�x�Ɉړ�������
	Vector3 velocity = { 0, 0, 1.5f };
	velocity = MatrixTransformDirection(velocity, matWorld);
	position += velocity;

	//�X�e�[�W�N���A��̍s�����������Ă����甲����
	if (isStageClearModeCompletion) { return; }

	//�^�C�}�[�X�V
	const float stageClearModeCompletionTime = 120;
	stageClearModeTimer++;

	//�^�C�}�[���w�肵�����ԂɈȉ��Ȃ甲����
	if (stageClearModeTimer < stageClearModeCompletionTime) { return; }

	//�X�e�[�W�N���A��̍s���������������Ƃɂ���
	isStageClearModeCompletion = true;
}
