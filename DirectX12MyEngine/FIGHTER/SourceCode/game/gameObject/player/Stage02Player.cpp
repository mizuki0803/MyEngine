#include "Stage02Player.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (Stage02Player::* Stage02Player::stageClearActionFuncTable[])() = {
	&Stage02Player::StageClearSideMove,
	&Stage02Player::StageClearReturn,
	&Stage02Player::StageClearUp,
	&Stage02Player::StageClearStay,
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
	//���@�̋��ʏ�����
	if (!Player::Initialize(model, startHP, maxHP)) {
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

	//�X�e�[�W�N���A�ړ����E���������߂�
	if (position.x < 0) { isStageClearMoveRight = false; }
	else { isStageClearMoveRight = true; }

	//�������x���Z�b�g
	stageClearMoveVelocity = { 0.2f, 0, 0.2f };
	//�����ړ����̂��߃}�C�i�X��n��
	if (!isStageClearMoveRight) {
		stageClearMoveVelocity.x = -stageClearMoveVelocity.x;
	}
	//��]���x���Z�b�g
	stageClearRota = rotation;

	//�p�[�e�B�N���̑傫���𓝈ꂷ�邽�߁A�ړ��͂������Ȃ����ʏ�ړ���Ԃɂ��Ă���
	moveSpeedPhase = MoveSpeedPhase::NormalSpeed;

	//�X�e�[�W�N���A��̓���������
	isStageClearMode = true;
}

void Stage02Player::StageClearReturnStart(const Vector3& cameraPos)
{
	//�J�������W���擾
	stageClearCameraPos = cameraPos;

	//�J�����z�[�~���O�p���W���Z�b�g
	cameraHomingPos = cameraPos;
	cameraHomingPos.y += 2.0f;

	//�������x���Z�b�g
	stageClearMoveVelocity = { 0, 0, 2.0f };
	//�A�ҏ�Ԃɂ���
	stageClearModePhase = StageClearModePhase::Return;

	//�^�C�}�[������
	stageClearModeTimer = 0;
}

void Stage02Player::StageClearBoostStart()
{
	//�u�[�X�g��Ԃɂ���
	stageClearModePhase = StageClearModePhase::Boost;
}

void Stage02Player::Crash()
{
	//���W�ړ�
	//�ė������x
	Vector3 crashAccel = { 0, -0.009f, 0 };
	crashVel += crashAccel;
	position += crashVel;

	//�ė���]����
	const Vector3 rotSpeed = { 0, 0.1f, 5 };
	//�o�E���h����܂ł�Z������]
	if (crashBoundCount == 0) { rotation.z += rotSpeed.z; }
	//1��o�E���h������Z���E��] & Y����]
	else if (crashBoundCount == 1) {
		rotation -= rotSpeed;
	}

	//�u���[�̋�����ʏ�ړ����̋����ɖ߂��Ă���
	SpeedChangeNormalBlur();

	//Y���W��0�ȉ��łȂ���Δ�����
	if (!(position.y <= 0)) { return; }
	//���W��0�ȉ��ɂȂ�Ȃ�
	position.y = 0;

	//�ė��o�E���h�񐔂𑝉�������
	crashBoundCount++;

	//�������o�p�ϐ�
	float explosionSize = 0;
	int explosionTime = 0;

	//�ė��o�E���h�񐔂����ڂȂ�o�E���h������
	if (crashBoundCount == 1) {
		//�o�E���h
		const float boundStartVel = fabsf(crashVel.y) * 0.8f;
		crashVel.y = boundStartVel;

		//�o�E���h�p�������o�p�ݒ�
		explosionSize = 0.5f;
		explosionTime = 20;
	}
	//�ė��o�E���h�񐔂����ڂȂ玀�S
	else if (crashBoundCount >= 2) {
		isDead = true;

		//���S�p�������o�p�ݒ�
		explosionSize = 3.5f;
		explosionTime = 60;
	}
	//�������o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->Explosion(position, explosionSize, explosionTime);
}

void Stage02Player::StageClear()
{
	//�X�e�[�W�N���A��s��
	(this->*stageClearActionFuncTable[static_cast<size_t>(stageClearModePhase)])();
}

void Stage02Player::StageClearSideMove()
{
	//�^�C�}�[�X�V
	stageClearModeTimer++;

	//�����x��ݒ�
	Vector3 accel = { 0.003f, 0, 0.002f };
	//��]�p�x���Z�b�g
	float maxRotZ = -(360 + 65);
	//�����ړ����̐ݒ�
	if (!isStageClearMoveRight) { accel.x = -accel.x;  maxRotZ = -maxRotZ; }

	//���x�ɉ����x�����Z
	stageClearMoveVelocity += accel;
	//���W���ړ�
	position += stageClearMoveVelocity;

	//��]
	const float rotTime = 120;
	float time = stageClearModeTimer / rotTime;
	time = min(time, 1);
	rotation.x = Easing::OutQuart(stageClearRota.x, 0, time);
	rotation.y = Easing::OutQuart(stageClearRota.y, 0, time);
	rotation.z = Easing::OutQuart(stageClearRota.z, maxRotZ, time);

	//�u���[�̋�����ʏ�ړ����̋����ɖ߂��Ă���
	SpeedChangeNormalBlur();
}

void Stage02Player::StageClearReturn()
{
	//�^�C�}�[�X�V
	const float maxTime = 250;
	stageClearModeTimer++;
	const float time = stageClearModeTimer / maxTime;

	//�w�肵�����Ԃ��߂�����
	float homingDistance = 100;
	const float distanceChangeStartTime = 120;
	if (stageClearModeTimer >= distanceChangeStartTime) {
		//�z�[�~���O�����ʒu�������炸�炷
		const float changeTime = (stageClearModeTimer - distanceChangeStartTime) / (maxTime - distanceChangeStartTime);
		homingDistance = Easing::LerpFloat(100, 0, changeTime);
	}
	cameraHomingPos.z = stageClearCameraPos.z + homingDistance;

	//�z�[�~���O�����ɍ��W���ړ�
	Vector3 toCamera = cameraHomingPos - GetWorldPos();
	toCamera.normalize();
	stageClearMoveVelocity.normalize();
	stageClearModeTimer = min(stageClearModeTimer, (int)maxTime);
	const float speed = 0.5f;
	stageClearMoveVelocity = Easing::LerpVec3(stageClearMoveVelocity, toCamera, time) * speed;
	position += stageClearMoveVelocity;

	//�i�s�����������悤�ɂ���
	rotation.y = XMConvertToDegrees(std::atan2(stageClearMoveVelocity.x, stageClearMoveVelocity.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 velocityZ = MatrixTransformDirection(stageClearMoveVelocity, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-velocityZ.y, velocityZ.z));

	//��]�p�xZ���Z�b�g
	const float downward = 180;
	float maxRotZ = (360 * 3 + downward);
	//�����ړ����̐ݒ�
	if (!isStageClearMoveRight) { maxRotZ = -maxRotZ; }
	rotation.z = Easing::OutQuad(stageClearRota.z, maxRotZ, time);

	//�J���������ւ������玟�̃t�F�[�Y��
	if (position.z <= stageClearCameraPos.z) {
		stageClearModePhase = StageClearModePhase::Up;

		//�p�x����]����������
		rotation.z = downward;

		//�^�C�}�[��������
		stageClearModeTimer = 0;
	}
}

void Stage02Player::StageClearUp()
{
	//�^�C�}�[�X�V
	const float upTime = 350;
	stageClearModeTimer++;

	//���������������
	const Vector3 rotSpeed = { -0.1f, 0, -1 };
	rotation += rotSpeed;
	const float upRotLimit = 40;
	rotation.x = max(rotation.x, -upRotLimit);
	rotation.z = max(rotation.z, 0);

	//���@���X���Ă���p�x�Ɉړ�������
	Vector3 velocity;
	velocity.x = stageClearMoveVelocity.x;
	velocity.y = stageClearMoveVelocity.z * (rotation.x / upRotLimit);
	velocity.z = stageClearMoveVelocity.z;
	position += velocity;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (stageClearModeTimer >= upTime) {
		stageClearModePhase = StageClearModePhase::Stay;

		//z��������]������
		isRotZRight = true;
		swayZ = 0.0f;

		//�^�C�}�[������
		stageClearModeTimer = 0;
	}
}

void Stage02Player::StageClearStay()
{
	//�������Ȃ��Ǝ₵���̂ł���炳���Ă���
	const float rotZSpeed = 0.03f;
	const float rotZLimit = 0.8f;
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
	rotation.z = swayZ;


	//���x��0��
	const float decelerationTime = 120;
	if (stageClearModeTimer >= decelerationTime) { return; }
	stageClearModeTimer++;
	const float time = stageClearModeTimer / decelerationTime;
	Vector3 velocity = Easing::LerpVec3(stageClearMoveVelocity, { 0, 0, 0 }, time);

	//���񂾂񌸑�
	position += velocity;
}

void Stage02Player::StageClearBoost()
{
	//�u�[�X�g�ړ�
	//���@���X���Ă���p�x�Ɉړ�������
	Vector3 velocity = { 0, 0, 1.25f };
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
