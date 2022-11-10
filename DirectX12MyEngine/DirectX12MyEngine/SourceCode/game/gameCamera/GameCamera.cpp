#include "GameCamera.h"
#include "Input.h"
#include "Easing.h"
#include "Player.h"
#include "BossMainBody.h"

void (GameCamera::* GameCamera::stageClearCameraActionFuncTable[])() = {
	&GameCamera::StageClearBossLook,
	&GameCamera::StageClearFrontLook,
	&GameCamera::StageClearPlayerLook,
	&GameCamera::StageClearPlayerZoom,
	&GameCamera::StageClearPlayerFollow,
	&GameCamera::StageClearPlayerSideMove,
	&GameCamera::StageClearPlayerKeepLook,
};

const float GameCamera::advanceSpeed = 0.3f;

void GameCamera::Initialize()
{
	//�������W��ݒ�
	position = { 0, 13, -30 };
	//������]�p��ݒ�
	rotation = { 0, 0, 0 };

	//�r���[�s��Ǝˉe�s��̏�����
	UpdateMatView();
	UpdateMatProjection();
}

void GameCamera::Update()
{
	//�J��������
	CameraAction();

	//��]�@���s�ړ��s��̌v�Z
	DirectX::XMMATRIX matRot, matTrans;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//���[���h�s��̍���
	matWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	matWorld *= matRot;		//���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f

	//���_�����[���h���W�ɐݒ�
	eye = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//�J�����̉�]�𔽉f������
	forward = MatrixTransformDirection(forward, matWorld);
	//���_����O���ɐi�񂾈ʒu�𒍎��_�ɐݒ�
	target = eye + forward;
	//�V�n�����]���Ă������悤�ɏ�����x�N�g������]������
	Vector3 baseUp(0, 1, 0);
	up = MatrixTransformDirection(baseUp, matWorld);

	//�V�F�C�N��ԂȂ�J�������V�F�C�N������
	if (isShake) {
		Shake();
	}

	//�r���[�s��Ǝˉe�s��̍X�V
	UpdateMatView();
	UpdateMatProjection();
}

void GameCamera::CrashStart()
{
	//�ė���Ԃɂ���
	isCrash = true;
	//�ė���Ԃ̃J�����ʒu�Ɉړ�����
	isMoveCrashPos = true;

	//�ė���Ԃ̃J�����ʒu�Ɉړ��O�̍��W���Z�b�g
	moveCrashBeforePos = position;
}

void GameCamera::ShakeStart()
{
	//�V�F�C�N�^�C�}�[�����Z�b�g
	shakeTimer = 0;
	//�V�F�C�N��Ԃɂ���
	isShake = true;
}

void GameCamera::StageClearModeStart(BossMainBody* bossMainBody)
{
	//�{�X�{�̂��Z�b�g
	this->bossMainBody = bossMainBody;
	//�{�X�̕����������Ƃ���Y���W���Z�b�g
	const float upperNum = 10.0f;
	bossLookPosY = bossMainBody->GetWorldPos().y + upperNum;
	//�ړ��O���W���Z�b�g
	stageClearMoveBeforePos = position;
	//�ړ��O�J�����p�x���Z�b�g
	stageClearMoveBeforeRota = rotation;

	//�X�e�[�W�N���A��̓���������
	isStageClearMode = true;
}

void GameCamera::BossDelete()
{
	//�X�e�[�W�N���A��̏�ԂłȂ���Δ�����
	if (!isStageClearMode) { return; }

	//�{�X�{�̂�����
	this->bossMainBody = nullptr;
	//�ړ��O�J�����p�x���Z�b�g
	stageClearMoveBeforeRota = rotation;
	//�X�e�[�W�N���A��̋����𐳖ʂ�������
	stageClearModePhase = StageClearModePhase::FrontLook;
	//�^�C�}�[������
	stageClearModeTimer = 0;
}

void GameCamera::CameraAction()
{
	//�X�e�[�W�N���A���
	if (isStageClearMode) {
		//�X�e�[�W�N���A��s��
		(this->*stageClearCameraActionFuncTable[static_cast<size_t>(stageClearModePhase)])();
	}
	//�ė����
	else if (isCrash) {
		Crash();
	}
	//����ȊO
	else {
		//��]
		Rotate();

		//�v���C���[���_���[�W��ԂȂ�m�b�N�o�b�N����
		if (player->GetIsDamage()) {
			Knockback();
		}
		//�_���[�W��ԂłȂ��Ȃ�ʏ�ړ�
		else {
			Move();
		}
	}
}

void GameCamera::Crash()
{
	//���@�̏�����ɃJ�������ړ�������
	const Vector3 crashCameraPos = { player->GetWorldPos().x, player->GetWorldPos().y + 3, 0 };
	if (isMoveCrashPos) {
		//�^�C�}�[���X�V
		const float moveCrashPosTime = 40;
		moveCrashPosTimer++;
		const float time = moveCrashPosTimer / moveCrashPosTime;

		position.x = Easing::OutQuad(moveCrashBeforePos.x, crashCameraPos.x, time);
		position.y = Easing::OutQuad(moveCrashBeforePos.y, crashCameraPos.y, time);

		//�^�C�}�[���w�肵�����ԂɂȂ�����ً}����I��
		if (moveCrashPosTimer >= moveCrashPosTime) {
			isMoveCrashPos = false;
		}
	}
	//�ړ��I������������W��Ǐ]��������
	else {
		//X,Y���W�͎��@�̏������Ǐ]����
		position.x = crashCameraPos.x;
		position.y = crashCameraPos.y;
	}

	//Z����]����
	const float rotSpeed = 1.5f;
	//���@�����o�E���h����܂ł�Z�����Ɉړ�����
	if (player->GetCrashBoundCount() == 0) {
		rotation.z += rotSpeed;
		position.z += 0.1f;
	}
	else if (player->GetCrashBoundCount() == 1) { rotation.z -= rotSpeed; }
}

void GameCamera::Rotate()
{
	//��](���[���J�����ɒǏ]���Ă��鎩�@�̌X���𗘗p����)
	rotation.x = player->GetRotation().x / 5;
	rotation.y = player->GetRotation().y / 5;
	rotation.z = -player->GetRotation().y / 8;
}

void GameCamera::Move()
{
	//�ړ����x
	Vector3 velocity;
	//�J�������X���Ă���p�x�Ɉړ�������
	const float moveSpeed = Player::GetMoveBaseSpeed() * 8;
	const Vector2 rotLimit = Player::GetRotLimit();
	velocity.x = moveSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveSpeed * -(rotation.x / rotLimit.x);

	//�O�i����ꍇ��Z�����Ɉړ�
	if (isAdvance) { velocity.z = advanceSpeed; }
	//�ړ�
	position += velocity;

	//�ړ����E����o�Ȃ��悤�ɂ���
	const Vector2 moveLimitMin = { Player::GetMoveLimitMin().x * 1.5f, 8.0f };
	const Vector2 moveLimitMax = { Player::GetMoveLimitMax().x * 1.5f, moveLimitMin.y + (Player::GetMoveLimitMax().y - Player::GetMoveLimitMin().y) * 1.5f };
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void GameCamera::Knockback()
{
	//���@�ɂ��킹�ăJ�������m�b�N�o�b�N������
	const float speed = Player::GetKnockbackBaseSpeed() * 8;
	Vector3 velocity = player->GetKnockbackVel();
	velocity *= speed;

	//�O�i����ꍇ��Z�����Ɉړ�
	if (isAdvance) { velocity.z = advanceSpeed; }
	//�ړ�
	position += velocity;

	//�ړ����E����o�Ȃ��悤�ɂ���
	const Vector2 moveLimitMin = { Player::GetMoveLimitMin().x * 1.5f, 8.0f };
	const Vector2 moveLimitMax = { Player::GetMoveLimitMax().x * 1.5f, moveLimitMin.y + (Player::GetMoveLimitMax().y - Player::GetMoveLimitMin().y) * 1.5f };
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void GameCamera::Shake()
{
	//�V�F�C�N���鎞�Ԃ�ݒ�
	const float shakeTime = 20;
	//�^�C�}�[���J�E���g
	shakeTimer++;
	const float time = shakeTimer / shakeTime;
	//�V�F�C�N�̍ő�̋�����ݒ�
	const float maxShakePower = 15;

	//�V�F�C�N����l���v�Z
	const float randShake = maxShakePower * (1 - time);
	Vector3 shake{};

	//�[�����Z������邽�߂�0�̏ꍇ�̓����_���𐶐����Ȃ�
	if (!((int)randShake == 0)) {
		shake.x = (float)((rand() % (int)randShake) - randShake / 2);
		shake.y = (float)((rand() % (int)randShake) - randShake / 2);
	}

	//�l���傫���̂Ŋ���Z���ď���������
	const float div = 100;
	shake /= div;

	//�J�����ɃV�F�C�N�̒l�����Z
	eye += shake;

	//�V�F�C�N������������V�F�C�N��Ԃ�����
	if (shakeTimer >= shakeTime) {
		isShake = false;
	}
}

void GameCamera::StageClearBossLook()
{
	//�{�X�{�̂̃C���X�^���X���Ȃ���Δ�����
	if (!bossMainBody) { return; }

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
	const Vector3 bossCameraVec = bossMainBody->GetWorldPos() - position;
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

void GameCamera::StageClearFrontLook()
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

void GameCamera::StageClearPlayerLook()
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
	if (player->GetStageClearModePhase() == Player::StageClearModePhase::Stay) {
		stageClearModePhase = StageClearModePhase::PlayerZoom;

		//�^�C�}�[������
		stageClearModeTimer = 0;
		//�ړ��O���W���Z�b�g
		stageClearMoveBeforePos = position;
	}
}

void GameCamera::StageClearPlayerZoom()
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

void GameCamera::StageClearPlayerFollow()
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

void GameCamera::StageClearPlayerSideMove()
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

void GameCamera::StageClearPlayerKeepLook()
{
	//���@�̕���������
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	rotation.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));
}
