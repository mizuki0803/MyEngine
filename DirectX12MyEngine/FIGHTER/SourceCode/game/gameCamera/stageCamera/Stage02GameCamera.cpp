#include "Stage02GameCamera.h"
#include "Easing.h"
#include "Stage02Player.h"
#include "Boss2Body.h"

void (Stage02GameCamera::* Stage02GameCamera::stageClearCameraActionFuncTable[])() = {
	&Stage02GameCamera::StageClearBossLook,
	&Stage02GameCamera::StageClearFrontLook,
	&Stage02GameCamera::StageClearPlayerLook,
	&Stage02GameCamera::StageClearPlayerZoom,
	&Stage02GameCamera::StageClearPlayerFollow,
	&Stage02GameCamera::StageClearPlayerSideMove,
	&Stage02GameCamera::StageClearPlayerKeepLook,
};

void Stage02GameCamera::Initialize(Stage02Player* player)
{
	//���@���Z�b�g
	this->player = player;

	//�������W��ݒ�
	const Vector3 playerPos = player->GetPosition();
	const Vector3 startPosition = { playerPos.x, playerPos.y + 10, -30 };
	position = startPosition;

	//�r���[�s��Ǝˉe�s��̏�����
	UpdateMatView();
	UpdateMatProjection();

	//�ړ����E���Z�b�g
	moveLimitMin = { player->GetMoveLimitMin().x * 1.5f, player->GetMoveLimitMin().y * 1.5f };
	moveLimitMax = { player->GetMoveLimitMax().x * 1.5f, player->GetMoveLimitMax().y * 1.5f };
}

void Stage02GameCamera::Update()
{
	//�J��������
	CameraAction(player);

	//�J�����̍X�V
	BaseGameCamera::Update();
}

void Stage02GameCamera::CrashStart()
{
	//�ė���Ԃɂ���
	isCrash = true;
	//�ė���Ԃ̃J�����ʒu�Ɉړ�����
	isMoveCrashPos = true;

	//�ė��p�̎��@�̒��S���W����̋���
	crashPlayerDistance = { 0, 0, -BasePlayer::GetBasePos().z };

	//�ė���Ԃ̃J�����ʒu�Ɉړ��O�̍��W���Z�b�g
	moveCrashBeforePos = position;
	//�ė���Ԃ̃J�����ʒu�Ɉړ��O�̊p�x���Z�b�g
	moveCrashBeforeRota = rotation;
	//�����p�x��0�ɂ��Ă���
	swayX = 0;
}

void Stage02GameCamera::StageClearModeStart(Boss2Body* boss2Body)
{
	//�{�X���̂��Z�b�g
	this->boss2Body = boss2Body;
	//�{�X�̕����������Ƃ���Y���W���Z�b�g
	const float upperNum = 10.0f;
	bossLookPosY = boss2Body->GetWorldPos().y + upperNum;
	//�ړ��O���W���Z�b�g
	stageClearMoveBeforePos = position;
	//�ړ��O�J�����p�x���Z�b�g
	stageClearMoveBeforeRota = rotation;
	//�����p�x��0�ɂ��Ă���
	swayX = 0;

	//�X�e�[�W�N���A��̓���������
	isStageClearMode = true;
}

void Stage02GameCamera::BossDelete()
{
	//�X�e�[�W�N���A��̏�ԂłȂ���Δ�����
	if (!isStageClearMode) { return; }

	//�{�X���̂�����
	this->boss2Body = nullptr;
	//�ړ��O�J�����p�x���Z�b�g
	stageClearMoveBeforeRota = rotation;
	//�X�e�[�W�N���A��̋����𐳖ʂ�������
	stageClearModePhase = StageClearModePhase::FrontLook;
	//�^�C�}�[������
	stageClearModeTimer = 0;
}

void Stage02GameCamera::Crash()
{	
	//���@�����S���Ă����甲����
	if (player->GetIsDead()) { return; }

	//���s�ړ��s��̌v�Z
	XMMATRIX matTrans = XMMatrixTranslation(crashPlayerDistance.x, crashPlayerDistance.y, crashPlayerDistance.z);

	//���[���h�s��̍���
	XMMATRIX bulletShotMatWorld;
	bulletShotMatWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	bulletShotMatWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f
	//���@�I�u�W�F�N�g�̃��[���h�s���������
	bulletShotMatWorld *= player->GetMatWorld();

	//�ė��p�J�������W�A��]�p���擾
	const Vector3 crashCameraPos = { bulletShotMatWorld.r[3].m128_f32[0], bulletShotMatWorld.r[3].m128_f32[1], bulletShotMatWorld.r[3].m128_f32[2] };
	const Vector3 crashCameraRota = { player->GetRotation().x,  player->GetRotation().y, 0 };

	//���@�̏�����ɃJ�������ړ�������
	if (isMoveCrashPos) {
		//�^�C�}�[���X�V
		const float moveCrashPosTime = 60;
		moveCrashPosTimer++;
		const float time = moveCrashPosTimer / moveCrashPosTime;

		position.x = Easing::OutQuad(moveCrashBeforePos.x, crashCameraPos.x, time);
		position.y = Easing::OutQuad(moveCrashBeforePos.y, crashCameraPos.y, time);
		position.z = Easing::OutQuad(moveCrashBeforePos.z, crashCameraPos.z, time);

		rotation.x = Easing::OutQuad(moveCrashBeforeRota.x, crashCameraRota.x, time);
		rotation.y = Easing::OutQuad(moveCrashBeforeRota.y, crashCameraRota.y, time);

		//�^�C�}�[���w�肵�����ԂɂȂ�����ً}����I��
		if (moveCrashPosTimer >= moveCrashPosTime) {
			isMoveCrashPos = false;
		}
	}
	//�ړ��I������������W��Ǐ]��������
	else {
		//�ė��p���W�Ɖ�]�p(���@�̌��)��Ǐ]��������
		position = crashCameraPos;
		rotation.x = crashCameraRota.x;
		rotation.y = crashCameraRota.y;

		//���񂾂�J�����Ǝ��@�̋����𗣂�
		crashPlayerDistance.z -= 0.1f;
	}

	//z�������邭���]�����Ă���
	const float rotZSpeed = -0.1f;
	rotation.z += rotZSpeed;
}

void Stage02GameCamera::StageClear()
{
	//�X�e�[�W�N���A��s��
	(this->*stageClearCameraActionFuncTable[static_cast<size_t>(stageClearModePhase)])();
}

void Stage02GameCamera::StageClearBossLook()
{
	//�{�X���̂̃C���X�^���X���Ȃ���Δ�����
	if (!boss2Body) { return; }

	//�{�X�̕�������������
	const float bossLookTime = 120;
	//�^�C�}�[�X�V
	stageClearModeTimer++;
	//�w��̎��Ԃ��z���Ȃ�
	stageClearModeTimer = min(stageClearModeTimer, (int)bossLookTime);
	const float time = stageClearModeTimer / bossLookTime;

	//���W����Ɉړ�
	position.y = Easing::OutCubic(stageClearMoveBeforePos.y, bossLookPosY, time);

	//X���W�����S�t�߂ɂ����獶�E�ɂ��炷
	const float xPosMin = 10.0f;
	if (fabsf(stageClearMoveBeforePos.x) <= xPosMin) {
		if (stageClearMoveBeforePos.x >= 0) { position.x = Easing::OutCubic(stageClearMoveBeforePos.x, xPosMin, time); }
		else { position.x = Easing::OutCubic(stageClearMoveBeforePos.x, -xPosMin, time); }
	}

	//�{�X�̕��������񂾂����
	const Vector3 bossCameraVec = boss2Body->GetWorldPos() - position;
	Vector3 rota = {};
	rota.y = XMConvertToDegrees(std::atan2(bossCameraVec.x, bossCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rota.y));
	Vector3 bossCameraVecZ = MatrixTransformDirection(bossCameraVec, matRot);
	rota.x = XMConvertToDegrees(std::atan2(-bossCameraVecZ.y, bossCameraVecZ.z));
	rotation.x = Easing::OutCubic(stageClearMoveBeforeRota.x, rota.x, time);
	rotation.y = Easing::OutCubic(stageClearMoveBeforeRota.y, rota.y, time);
	rotation.z = Easing::OutCubic(stageClearMoveBeforeRota.z, rota.z, time);
}

void Stage02GameCamera::StageClearFrontLook()
{
	//������؂�ւ��鎞��
	const float lookChangeTime = 300;
	//�^�C�}�[�X�V
	stageClearModeTimer++;
	stageClearModeTimer = min(stageClearModeTimer, (int)lookChangeTime);
	const float time = stageClearModeTimer / lookChangeTime;

	//���ʂ�����
	rotation.x = Easing::InOutCubic(stageClearMoveBeforeRota.x, 0, time);
	rotation.y = Easing::InOutCubic(stageClearMoveBeforeRota.y, 0, time);
	rotation.z = Easing::InOutCubic(stageClearMoveBeforeRota.z, 0, time);

	//���@���J�����̌��ɂ������玟�̃t�F�[�Y��
	const float changePhaseDistance = 10.0f;
	if (player->GetWorldPos().z <= position.z + changePhaseDistance) {
		stageClearModePhase = StageClearModePhase::PlayerLook;

		//�ړ��O�J�����p�x���Z�b�g
		stageClearMoveBeforeRota = rotation;
		//�^�C�}�[������
		stageClearModeTimer = 0;
	}
}

void Stage02GameCamera::StageClearPlayerLook()
{
	//������؂�ւ��鎞��
	const float lookChangeTime = 20;
	//�^�C�}�[�X�V
	stageClearModeTimer++;
	//�w��̎��Ԃ��z���Ȃ�
	stageClearModeTimer = min(stageClearModeTimer, (int)lookChangeTime);
	const float time = stageClearModeTimer / lookChangeTime;

	//���@�̕��������񂾂����
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	Vector3 rota = {};
	rota.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rota.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rota.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));
	rotation.x = Easing::InOutCubic(stageClearMoveBeforeRota.x, rota.x, time);
	rotation.y = Easing::InOutCubic(stageClearMoveBeforeRota.y, rota.y, time);
	rotation.z = Easing::InOutCubic(stageClearMoveBeforeRota.z, 0, time);

	//���@����~��Ԃɓ������玟�̃t�F�[�Y��
	if (player->GetStageClearModePhase() == Stage02Player::StageClearModePhase::Stay) {
		stageClearModePhase = StageClearModePhase::PlayerZoom;

		//�^�C�}�[������
		stageClearModeTimer = 0;
		//�ړ��O���W���Z�b�g
		stageClearMoveBeforePos = position;
	}
}

void Stage02GameCamera::StageClearPlayerZoom()
{
	//�Y�[�����鎞��
	const float zoomTime = 300;
	//�^�C�}�[�X�V
	stageClearModeTimer++;
	const float time = stageClearModeTimer / zoomTime;

	//���@�̒��S���W����̋���
	const Vector3 distancePos = { 0, 0.5f, -10.0f };
	//���s�ړ��s��̌v�Z
	XMMATRIX matTrans = XMMatrixTranslation(distancePos.x, distancePos.y, distancePos.z);
	//���@�̌�냏�[���h�s��
	XMMATRIX playerBackMatWorld;
	playerBackMatWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	playerBackMatWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f
	//���@�̃��[���h�s���������
	playerBackMatWorld *= player->GetMatWorld();
	//���@�̌����W���擾
	Vector3 playerBack = { playerBackMatWorld.r[3].m128_f32[0], playerBackMatWorld.r[3].m128_f32[1], playerBackMatWorld.r[3].m128_f32[2] };

	//���@�̌��ɃY�[������
	position.x = Easing::InOutQuad(stageClearMoveBeforePos.x, playerBack.x, time);
	position.y = Easing::InOutQuad(stageClearMoveBeforePos.y, playerBack.y, time);
	position.z = Easing::InOutQuad(stageClearMoveBeforePos.z, playerBack.z, time);

	//���@�̕���������
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	rotation.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));

	//�w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (stageClearModeTimer >= zoomTime) {
		stageClearModePhase = StageClearModePhase::PlayerFollow;

		//�^�C�}�[������
		stageClearModeTimer = 0;
	}
}

void Stage02GameCamera::StageClearPlayerFollow()
{
	//�Ǐ]���鎞��
	const float followTime = 90;
	//�^�C�}�[�X�V
	stageClearModeTimer++;
	const float time = stageClearModeTimer / followTime;

	//�w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (stageClearModeTimer >= followTime) {
		stageClearModePhase = StageClearModePhase::PlayerSideMove;

		//�^�C�}�[������
		stageClearModeTimer = 0;

		//�ړ��O���W���Z�b�g
		stageClearMoveBeforePos = position;
	}
}

void Stage02GameCamera::StageClearPlayerSideMove()
{
	//�ړ����鎞��
	const float moveTime = 300;

	//���@�̕���������
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	rotation.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));

	//�^�C�}�[�X�V
	stageClearModeTimer++;
	float time = stageClearModeTimer / moveTime;

	//���@�̒��S���W����̋���
	const Vector3 distancePos = { 10.0f, 0, -4.0f };
	//���s�ړ��s��̌v�Z
	XMMATRIX matTrans = XMMatrixTranslation(distancePos.x, distancePos.y, distancePos.z);
	//���@�����[���h�s��
	XMMATRIX playerSideMatWorld;
	playerSideMatWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	playerSideMatWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f
	//���@�̃��[���h�s���������
	playerSideMatWorld *= player->GetMatWorld();
	//���@�����W���擾(Y���W�͈ړ��O��菭����)
	const float upNum = 0.5f;
	Vector3 playerSide = { playerSideMatWorld.r[3].m128_f32[0], stageClearMoveBeforePos.y + upNum, playerSideMatWorld.r[3].m128_f32[2] };

	//���@���Ɉړ�����
	position.x = Easing::OutQuad(stageClearMoveBeforePos.x, playerSide.x, time);
	position.y = Easing::OutQuad(stageClearMoveBeforePos.y, playerSide.y, time);
	position.z = Easing::OutQuad(stageClearMoveBeforePos.z, playerSide.z, time);

	//�w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (stageClearModeTimer >= moveTime) {
		stageClearModePhase = StageClearModePhase::PlayerKeepLook;
	}
}

void Stage02GameCamera::StageClearPlayerKeepLook()
{
	//���@�̕���������
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	rotation.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));
}
