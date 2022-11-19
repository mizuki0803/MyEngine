#include "StageSelectCamera.h"
#include "StageSelectPlayer.h"
#include "Easing.h"

void (StageSelectCamera::* StageSelectCamera::actionFuncTable[])() = {
	&StageSelectCamera::GooutPlayerLookAction,
	&StageSelectCamera::StageSelectStartAction,
	&StageSelectCamera::StageSelectAction,
	&StageSelectCamera::StageDecisionCheckAction,
	&StageSelectCamera::ReturnSelectAction,
};

const Vector3 StageSelectCamera::gooutPlayerLookSelectFieldDistance = { 0, 5, -40 };
const Vector3 StageSelectCamera::stageSelectPlayerDistance = { 0, 25, -50 };
const Vector3 StageSelectCamera::stageDecisionCheckPlayerDistance = { 0, 0.5f, -20 };

void StageSelectCamera::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//�J�����X�V
	Camera::Update();
}

void StageSelectCamera::GooutPlayerLookActionStart(const Vector3& gooutPlanetPos, const Vector3& selectFieldPos)
{
	//�^�C�}�[������
	cameraActionTimer = 0;
	//���@���o�Ă���f���̍��W���L��
	this->gooutPlanetPos = gooutPlanetPos;

	//�J�������_�����@���o�Ă���f���̃t�B�[���h����J�����̋����ɐݒ�
	eye = selectFieldPos + gooutPlayerLookSelectFieldDistance;

	//�J�����̒����_���o�Ă���f���ɐݒ�
	target = gooutPlanetPos;

	//�f������o�Ă��鎩�@�������ԂɕύX
	phase = ActionPhase::GooutPlayerLook;
}

void StageSelectCamera::StageSelectStartActionStart()
{
	//�^�C�}�[������
	cameraActionTimer = 0;

	//�ړ��O�̎��_�ƒ����_���Z�b�g
	moveBeforeEye = eye;
	moveBeforeTarget = target;

	//�X�e�[�W�I���J�n��ԂɕύX
	phase = ActionPhase::StageSelectStart;
}

void StageSelectCamera::StageDecisionCheckActionStart()
{
	//�^�C�}�[������
	cameraActionTimer = 0;

	//�X�e�[�W����m�F��ԂɕύX
	phase = ActionPhase::StageDecisionCheck;
}

void StageSelectCamera::ReturnSelectActionStart()
{
	//�^�C�}�[������
	cameraActionTimer = 0;

	//�X�e�[�W�I���ɖ߂��ԂɕύX
	phase = ActionPhase::ReturnSelect;
}

void StageSelectCamera::GooutPlayerLookAction()
{
}

void StageSelectCamera::StageSelectStartAction()
{
	//�J�����ړ������鎞��
	const float cameraMoveTime = 60;
	//�^�C�}�[�X�V
	cameraActionTimer++;
	const float time = cameraActionTimer / cameraMoveTime;

	//�J�������_���C�[�W���O�œ�����
	const Vector3 afterEye = player->GetPosition() + stageSelectPlayerDistance;
	eye.x = Easing::OutQuad(moveBeforeEye.x, afterEye.x, time);
	eye.y = Easing::OutQuad(moveBeforeEye.y, afterEye.y, time);
	eye.z = Easing::OutQuad(moveBeforeEye.z, afterEye.z, time);
	//�J���������_���C�[�W���O�œ�����
	const Vector3 afterTarget = player->GetPosition();
	target.x = Easing::OutQuad(moveBeforeTarget.x, afterTarget.x, time);
	target.y = Easing::OutQuad(moveBeforeTarget.y, afterTarget.y, time);
	target.z = Easing::OutQuad(moveBeforeTarget.z, afterTarget.z, time);

	dirtyView = true;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (cameraActionTimer >= cameraMoveTime) {
		//�X�e�[�W�I����ԂɕύX
		phase = ActionPhase::StageSelect;
	}
}

void StageSelectCamera::StageSelectAction()
{
	//�J�������_���X�e�[�W�I����Ԃŕ������鎩�@�ƃJ�����̋����ɐݒ�
	eye = player->GetPosition() + stageSelectPlayerDistance;

	//�J�����̒����_�����@�ɐݒ�
	target = player->GetPosition();
	dirtyView = true;
}

void StageSelectCamera::StageDecisionCheckAction()
{
	//�J�����ړ������鎞��
	const float cameraMoveTime = 40;

	//�^�C�}�[���w�肵�����Ԉȏ�Ȃ甲����
	if (cameraActionTimer >= cameraMoveTime) { return; }
	//�^�C�}�[�X�V
	cameraActionTimer++;
	const float time = cameraActionTimer / cameraMoveTime;

	//�J�������_���X�e�[�W����m�F��Ԃ̎��@�ƃJ�����̋����ɓ�����
	Vector3 playerDistance;
	playerDistance.x = Easing::OutQuad(stageSelectPlayerDistance.x, stageDecisionCheckPlayerDistance.x, time);
	playerDistance.y = Easing::OutQuad(stageSelectPlayerDistance.y, stageDecisionCheckPlayerDistance.y, time);
	playerDistance.z = Easing::OutQuad(stageSelectPlayerDistance.z, stageDecisionCheckPlayerDistance.z, time);
	eye = player->GetPosition() + playerDistance;

	//�J�����̒����_�����@�ɐݒ�
	target = player->GetPosition();
	dirtyView = true;
}

void StageSelectCamera::ReturnSelectAction()
{
	//�J�����ړ������鎞��
	const float cameraMoveTime = 40;
	//�^�C�}�[�X�V
	cameraActionTimer++;
	const float time = cameraActionTimer / cameraMoveTime;

	//�J�������_���X�e�[�W�I����Ԃ̎��@�ƃJ�����̋����ɓ�����
	Vector3 playerDistance;
	playerDistance.x = Easing::OutQuad(stageDecisionCheckPlayerDistance.x, stageSelectPlayerDistance.x, time);
	playerDistance.y = Easing::OutQuad(stageDecisionCheckPlayerDistance.y, stageSelectPlayerDistance.y, time);
	playerDistance.z = Easing::OutQuad(stageDecisionCheckPlayerDistance.z, stageSelectPlayerDistance.z, time);
	eye = player->GetPosition() + playerDistance;

	//�J�����̒����_�����@�ɐݒ�
	target = player->GetPosition();
	dirtyView = true;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (cameraActionTimer >= cameraMoveTime) {
		//�X�e�[�W�I����ԂɕύX
		phase = ActionPhase::StageSelect;
	}
}
