#include "Stage02SortieCamera.h"
#include "Stage02SortiePlayer.h"
#include "Easing.h"

void (Stage02SortieCamera::* Stage02SortieCamera::sortieActionFuncTable[])() = {
	&Stage02SortieCamera::LookPlayer,
	&Stage02SortieCamera::ZoomPlayer,
	&Stage02SortieCamera::RunningSidePlayer,
	&Stage02SortieCamera::Stay,
};

const Vector3 Stage02SortieCamera::playerDistance = { 3, 1, -15 };

void Stage02SortieCamera::Initialize()
{
	//�J����������
	Camera::Initialize();

	//�J�����̏����l���Z�b�g
	const Vector3 startEyePos = { 35, 1, 0 };
	eye = startEyePos;
	moveBeforeEye = eye;
}

void Stage02SortieCamera::Update()
{
	//�s��
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieMovePhase)])();

	//�J�����X�V
	Camera::Update();
}

void Stage02SortieCamera::LookPlayer()
{
	//���@�̕����������̂ɂ����鎞��
	const float lookTime = 400;
	//�^�C�}�[�X�V
	cameraActionTimer++;
	const float time = cameraActionTimer / lookTime;

	//�J�����̒����_�����@�����Ɉړ�������
	const Vector3 startTargetPos = { 0, 1, eye.z + 60 };
	const Vector3 playerPos = player->GetPosition();
	target.x = Easing::InOutQuad(startTargetPos.x, playerPos.x, time);
	target.y = Easing::InOutQuad(startTargetPos.y, playerPos.y, time);
	target.z = Easing::InOutQuad(startTargetPos.z, playerPos.z, time);
	dirtyView = true;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (cameraActionTimer >= lookTime) {
		//���̃t�F�[�Y��
		sortieMovePhase = Stage02SortieActionPhase::ZoomPlayer;

		//�^�C�}�[��������
		cameraActionTimer = 0;
	}
}

void Stage02SortieCamera::ZoomPlayer()
{
	//�Y�[���ɂ����鎞��
	const float zoomTime = 180;
	//�^�C�}�[�X�V
	cameraActionTimer++;
	const float time = cameraActionTimer / zoomTime;

	//�J���������@�ɃY�[��������
	const Vector3 playerPos = player->GetPosition();
	const Vector3 moveAfterPos = playerPos + playerDistance;
	eye.x = Easing::OutQuad(moveBeforeEye.x, moveAfterPos.x, time);
	eye.y = Easing::OutQuad(moveBeforeEye.y, moveAfterPos.y, time);
	eye.z = Easing::OutQuad(moveBeforeEye.z, moveAfterPos.z, time);
	target = playerPos;
	dirtyView = true;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (cameraActionTimer >= zoomTime) {
		//���̃t�F�[�Y��
		sortieMovePhase = Stage02SortieActionPhase::RunningSide;

		//�^�C�}�[��������
		cameraActionTimer = 0;
	}
}

void Stage02SortieCamera::RunningSidePlayer()
{
	//��������
	const float runningTime = 215;
	//�^�C�}�[�X�V
	cameraActionTimer++;

	//�J���������@�ɕ���������
	const Vector3 playerPos = player->GetPosition();
	eye = playerPos + playerDistance;
	target = playerPos;
	dirtyView = true;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (cameraActionTimer >= runningTime) {
		//���̃t�F�[�Y��
		sortieMovePhase = Stage02SortieActionPhase::Stay;

		//����&�Y�[�����I��������
		isZoomEnd = true;
	}
}

void Stage02SortieCamera::Stay()
{
}
