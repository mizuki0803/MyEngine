#include "Stage02GameCamera.h"
#include "Easing.h"
#include "Stage02Player.h"
#include "Boss2Body.h"

void (Stage02GameCamera::* Stage02GameCamera::stageClearCameraActionFuncTable[])() = {
	&Stage02GameCamera::StageClearBossSide,
	&Stage02GameCamera::StageClearBossBack,
	&Stage02GameCamera::StageClearPlayerFront,
	&Stage02GameCamera::StageClearPlayerAround,
	&Stage02GameCamera::StageClearPlayerBoostLookPos,
};

void Stage02GameCamera::Initialize(Stage02Player* player)
{
	//���@���Z�b�g
	this->player = player;

	//�������W��ݒ�
	const Vector3 playerDistance = { 0, 2, -30 };
	const Vector3 startPosition = player->GetPosition() + playerDistance;
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
	crashPlayerDistance = { 0, 0, -player->GetBasePos().z };

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
	//�ړ��O���W���Z�b�g
	stageClearMoveBeforePos = position;
	//�ړ��O�J�����p�x���Z�b�g
	stageClearMoveBeforeRota = rotation;
	//�����p�x��0�ɂ��Ă���
	swayX = 0;

	//�X�e�[�W�N���A��̓���������
	isStageClearMode = true;
}

void Stage02GameCamera::StageClearPlayerFrontStart()
{
	//�X�e�[�W�N���A��̏�ԂłȂ���Δ�����
	if (!isStageClearMode) { return; }

	//�ړ��O���W���Z�b�g
	stageClearMoveBeforePos = position;
	//�ړ��O�J�����p�x���Z�b�g
	stageClearMoveBeforeRota = rotation;
	if (stageClearMoveBeforeRota.y < 0) { stageClearMoveBeforeRota.y += 360; } //�p�x�C��

	//�X�e�[�W�N���A��̍s�������@�̐��ʂɈړ���
	stageClearModePhase = StageClearModePhase::PlayerFront;
	//�^�C�}�[������
	stageClearModeTimer = 0;
}

void Stage02GameCamera::StageClearPlayerBoostLookPosStart()
{
	//�ړ��O�̎��@��������Ƃ��̉�]�p�x���Z�b�g(�񂷂��߂Ɉ�����̑��₷)
	beforePlayerAroundRotAngle = playerAroundRotAngle - 360;
	//�ړ��O�̎��@��������Ƃ��̎��@�Ƃ̋������Z�b�g
	beforePlayerAroundDistance = playerAroundDistance;
	//�ړ��O���W���Z�b�g
	stageClearMoveBeforePos = position;

	//�X�e�[�W�N���A��̍s�������@�̃u�[�X�g������ʒu�Ɉړ���
	stageClearModePhase = StageClearModePhase::PlayerBoostPos;
	//�^�C�}�[������
	stageClearModeTimer = 0;
}

void Stage02GameCamera::BossDelete()
{
	//�X�e�[�W�N���A��̏�ԂłȂ���Δ�����
	if (!isStageClearMode) { return; }

	//�{�X���̂�����
	this->boss2Body = nullptr;
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

void Stage02GameCamera::StageClearBossSide()
{
	//�{�X���̂̃C���X�^���X���Ȃ���Δ�����
	if (!boss2Body) { return; }

	//�{�X�̉��Ɉړ�����̂ɂ����鎞��
	const float sideMoveTime = 240;
	//�^�C�}�[�X�V
	stageClearModeTimer++;
	//�w��̎��Ԃ��z���Ȃ�
	stageClearModeTimer = min(stageClearModeTimer, (int)sideMoveTime);
	const float time = stageClearModeTimer / sideMoveTime;

	//���W���{�X���Ɉړ�
	const Vector3 bossDistance = { 45, -1, 0 };
	const Vector3 bossSidePos = boss2Body->GetPosition() + bossDistance;
	position.x = Easing::InOutQuad(stageClearMoveBeforePos.x, bossSidePos.x, time);
	position.y = Easing::InOutQuad(stageClearMoveBeforePos.y, bossSidePos.y, time);
	position.z = Easing::InBack(stageClearMoveBeforePos.z, bossSidePos.z, time);

	//�{�X�̕��������񂾂����
	const Vector3 bossCameraVec = boss2Body->GetWorldPos() - position;
	Vector3 rota = {};
	rota.y = XMConvertToDegrees(std::atan2(bossCameraVec.x, bossCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rota.y));
	Vector3 bossCameraVecZ = MatrixTransformDirection(bossCameraVec, matRot);
	rota.x = XMConvertToDegrees(std::atan2(-bossCameraVecZ.y, bossCameraVecZ.z));
	rotation.x = Easing::InQuad(stageClearMoveBeforeRota.x, rota.x, time);
	rotation.y = Easing::InQuad(stageClearMoveBeforeRota.y, rota.y, time);
	rotation.z = Easing::InQuad(stageClearMoveBeforeRota.z, rota.z, time);

	//�w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (stageClearModeTimer >= sideMoveTime) {
		stageClearModePhase = StageClearModePhase::BossBack;

		//�^�C�}�[������
		stageClearModeTimer = 0;
		//�ړ��O���W���Z�b�g
		stageClearMoveBeforePos = position;
	}
}

void Stage02GameCamera::StageClearBossBack()
{
	//�{�X���̂̃C���X�^���X���Ȃ���Δ�����
	if (!boss2Body) { return; }

	//�{�X�̌��Ɉړ�����̂ɂ����鎞��
	const float backMoveTime = 260;
	//�^�C�}�[�X�V
	stageClearModeTimer++;
	//�w��̎��Ԃ��z���Ȃ�
	stageClearModeTimer = min(stageClearModeTimer, (int)backMoveTime);
	const float time = stageClearModeTimer / backMoveTime;

	//���W���{�X���Ɉړ�
	const Vector3 bossDistance = { 0, -1, 80 };
	const Vector3 bossSidePos = boss2Body->GetPosition() + bossDistance;
	position.x = Easing::OutQuad(stageClearMoveBeforePos.x, bossSidePos.x, time);
	position.y = Easing::OutQuad(stageClearMoveBeforePos.y, bossSidePos.y, time);
	position.z = Easing::OutQuad(stageClearMoveBeforePos.z, bossSidePos.z, time);

	//�{�X�̕���������
	const Vector3 bossCameraVec = boss2Body->GetWorldPos() - position;
	rotation.y = XMConvertToDegrees(std::atan2(bossCameraVec.x, bossCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 bossCameraVecZ = MatrixTransformDirection(bossCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-bossCameraVecZ.y, bossCameraVecZ.z));
}

void Stage02GameCamera::StageClearPlayerFront()
{
	//���@�̐��ʂɈړ�����̂ɂ����鎞��
	const float playerFrontMoveTime = 240;
	//�^�C�}�[�X�V
	stageClearModeTimer++;
	//�w��̎��Ԃ��z���Ȃ�
	stageClearModeTimer = min(stageClearModeTimer, (int)playerFrontMoveTime);
	const float time = stageClearModeTimer / playerFrontMoveTime;

	//���@�̒��S���W����̋���
	const Vector3 distancePos = { 0, -0.5f, 15.0f };
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

	//���@�̕��������񂾂����
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	Vector3 rota = {};
	rota.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	if (rota.y <= 0) { rota.y += 360; } //�p�x�C��
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rota.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rota.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));
	rotation.x = Easing::OutCubic(stageClearMoveBeforeRota.x, rota.x, time);
	rotation.y = Easing::OutCubic(stageClearMoveBeforeRota.y, rota.y, time);
	rotation.z = Easing::OutCubic(stageClearMoveBeforeRota.z, rota.z, time);

	//�w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (stageClearModeTimer >= playerFrontMoveTime) {
		stageClearModePhase = StageClearModePhase::PlayerAround;

		//�^�C�}�[������
		stageClearModeTimer = 0;
		//�ړ��O���W���Z�b�g
		stageClearMoveBeforePos = position;
		//���@�Ƃ̋������L�^
		playerAroundDistance = fabsf(playerCameraVec.z);
	}
}

void Stage02GameCamera::StageClearPlayerAround()
{
	//���@���W
	const Vector3 playerPos = player->GetWorldPos();

	//�p�x��ύX��������
	const float rotSpeed = 0.3f;
	playerAroundRotAngle -= rotSpeed;
	//360���z���Ȃ��悤�ɂ���
	if (playerAroundRotAngle <= -360) { playerAroundRotAngle += 360; }

	//���@��������
	const float radian = XMConvertToRadians(playerAroundRotAngle);
	position.x = playerAroundDistance * sinf(radian) + playerPos.x;
	position.z = playerAroundDistance * cosf(radian) + playerPos.z;

	//���@�̕���������
	const Vector3 playerCameraVec = playerPos - position;
	rotation.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));
}

void Stage02GameCamera::StageClearPlayerBoostLookPos()
{
	//���@�̕���������
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	rotation.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));

	//�X�e�[�W�N���A����������鎞��
	const int stageClearModeCompletionTime = 300;
	//�^�C�}�[�X�V
	stageClearModeTimer++;
	//�^�C�}�[���w�肵�����ԂɂȂ����犮���t���O�𗧂Ă�
	if (stageClearModeTimer > stageClearModeCompletionTime) {
		isStageClearModeCompletion = true;

		//������
		return;
	}

	//���@�ɕ���������
	const float radian = XMConvertToRadians(playerAroundRotAngle);
	const Vector3 playerPos = player->GetWorldPos();//���@���W
	position.x = playerAroundDistance * sinf(radian) + playerPos.x;
	position.z = playerAroundDistance * cosf(radian) + playerPos.z;

	//���@����1���ɂ����鎞��
	const float rotRoundTime = 240;
	if (stageClearModeTimer > rotRoundTime) { return; }
	const float time = stageClearModeTimer / rotRoundTime;

	//���@�̎�������������
	playerAroundRotAngle = Easing::OutQuart(beforePlayerAroundRotAngle, -165, time);
	playerAroundDistance = Easing::OutQuart(beforePlayerAroundDistance, 17.5f, time);
	position.y = Easing::OutQuart(stageClearMoveBeforePos.y, playerPos.y + 6, time);
}
