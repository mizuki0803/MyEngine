#include "SortieCamera.h"
#include "SortiePlayer.h"
#include "Easing.h"

void (SortieCamera::* SortieCamera::sortieActionFuncTable[])() = {
	&SortieCamera::StayPlayer,
	&SortieCamera::RunningSideZoomPlayer,
	&SortieCamera::CameraRunningSideSpeedMove,
};

float SortieCamera::runningSideStartDistance = 40.0f;

void SortieCamera::Initialize()
{
	//�J����������
	Camera::Initialize();

	//�J�����̎��_�����@�̌������ɃZ�b�g
	eyePlayerDistance = { 0, 2, 400 };
	eye = player->GetPosition() + eyePlayerDistance;
}

void SortieCamera::Update()
{
	//�s��
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieMovePhase)])();

	//�J�����X�V
	Camera::Update();
}

void SortieCamera::StayPlayer()
{
	//�J�����̒����_�����@�����Ɉړ�������
	target = player->GetPosition();
	dirtyView = true;

	//���@�Ƃ̋������������J�n���鋗���ɂȂ�����
	eyePlayerDistance.z = eye.z - player->GetPosition().z;
	if (eyePlayerDistance.z <= runningSideStartDistance) {
		//���̃t�F�[�Y��
		sortieMovePhase = SortieActionPhase::RunningSideZoom;
	}
}

void SortieCamera::RunningSideZoomPlayer()
{
	//���@���Y�[�����鎞��
	const float zoomTime = 300;
	//�^�C�}�[�X�V
	cameraActionTimer++;
	const float time = cameraActionTimer / zoomTime;

	//�����I�����̎��@�ƃJ�����̋���
	const float runningSideEndDistance = 15.0f;
	//�J���������@�ɕ���������
	eyePlayerDistance.z = Easing::OutQuint(runningSideStartDistance, runningSideEndDistance, time);
	eye = player->GetPosition() + eyePlayerDistance;

	//�J�����̒����_�����@�����Ɉړ�������
	target = player->GetPosition();
	dirtyView = true;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (cameraActionTimer >= zoomTime) {
		//���̃t�F�[�Y��
		sortieMovePhase = SortieActionPhase::RunningSideSpeedMove;

		//����&�Y�[�����I��������
		isZoomEnd = true;
	}
}

void SortieCamera::CameraRunningSideSpeedMove()
{
	//�����I�����̎��@�ƃJ�����̋���
	const float runningSideEndDistance = 15.0f;
	//�J���������@�ɕ���������
	eye.z += 1.0f;

	//�J�����̒����_�����@�����Ɉړ�������
	const Vector3 playerPos = player->GetPosition();
	target = { playerPos.x, playerPos.y, eye.z - runningSideEndDistance };
	
	dirtyView = true;
}
