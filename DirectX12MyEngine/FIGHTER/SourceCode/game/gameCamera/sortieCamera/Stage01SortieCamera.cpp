#include "Stage01SortieCamera.h"
#include "Stage01SortiePlayer.h"
#include "Easing.h"

void (Stage01SortieCamera::* Stage01SortieCamera::sortieActionFuncTable[])() = {
	&Stage01SortieCamera::StayPlayer,
	&Stage01SortieCamera::RunningSideZoomPlayer,
	&Stage01SortieCamera::CameraRunningSideSpeedMove,
};

const float Stage01SortieCamera::runningSideStartDistance = 45.0f;
const float Stage01SortieCamera::runningSideEndDistance = 20.0f;

void Stage01SortieCamera::Initialize()
{
	//�J����������
	Camera::Initialize();

	//�J�����̎��_�����@�̌������ɃZ�b�g
	eyePlayerDistance = { 0, 1.5f, 350 };
	eye = player->GetPosition() + eyePlayerDistance;
}

void Stage01SortieCamera::Update()
{
	//�s��
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieMovePhase)])();

	//�J�����X�V
	Camera::Update();
}

void Stage01SortieCamera::StayPlayer()
{
	//�J�����̒����_�����@�����Ɉړ�������
	target = player->GetPosition();
	dirtyView = true;

	//���@�Ƃ̋������������J�n���鋗���ɂȂ�����
	eyePlayerDistance.z = eye.z - player->GetPosition().z;
	if (eyePlayerDistance.z <= runningSideStartDistance) {
		//���̃t�F�[�Y��
		sortieMovePhase = Stage01SortieActionPhase::RunningSideZoom;
	}
}

void Stage01SortieCamera::RunningSideZoomPlayer()
{
	//���@���Y�[�����鎞��
	const float zoomTime = 240;
	//�^�C�}�[�X�V
	cameraActionTimer++;
	const float time = cameraActionTimer / zoomTime;

	//�J���������@�ɕ���������
	eyePlayerDistance.z = Easing::OutQuint(runningSideStartDistance, runningSideEndDistance, time);
	eye = player->GetPosition() + eyePlayerDistance;

	//�J�����̒����_�����@�����Ɉړ�������
	target = player->GetPosition();
	dirtyView = true;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (cameraActionTimer >= zoomTime) {
		//���̃t�F�[�Y��
		sortieMovePhase = Stage01SortieActionPhase::RunningSideSpeedMove;

		//����&�Y�[�����I��������
		isZoomEnd = true;
	}
}

void Stage01SortieCamera::CameraRunningSideSpeedMove()
{
	//�J���������@�̒ʏ�X�s�[�h�ŕ���������
	eye.z += Stage01SortiePlayer::GetAdvanceSpeed();

	//�J�����̒����_�����@�����Ɉړ�������
	const Vector3 playerPos = player->GetPosition();
	target = { playerPos.x, playerPos.y, eye.z - runningSideEndDistance };
	dirtyView = true;
}
