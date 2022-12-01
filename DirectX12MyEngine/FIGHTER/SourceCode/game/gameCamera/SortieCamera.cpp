#include "SortieCamera.h"
#include "SortiePlayer.h"
#include "Easing.h"

void (SortieCamera::* SortieCamera::sortieActionFuncTable[])() = {
	&SortieCamera::StayPlayer,
	&SortieCamera::RunningSideZoomPlayer,
	&SortieCamera::CameraRunningSideSpeedMove,
};

const float SortieCamera::runningSideStartDistance = 45.0f;
const float SortieCamera::runningSideEndDistance = 20.0f;

void SortieCamera::Initialize()
{
	//カメラ初期化
	Camera::Initialize();

	//カメラの視点を自機の後方左上にセット
	eyePlayerDistance = { 0, 1.5f, 350 };
	eye = player->GetPosition() + eyePlayerDistance;
}

void SortieCamera::Update()
{
	//行動
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieMovePhase)])();

	//カメラ更新
	Camera::Update();
}

void SortieCamera::StayPlayer()
{
	//カメラの注視点を自機方向に移動させる
	target = player->GetPosition();
	dirtyView = true;

	//自機との距離が並走を開始する距離になったら
	eyePlayerDistance.z = eye.z - player->GetPosition().z;
	if (eyePlayerDistance.z <= runningSideStartDistance) {
		//次のフェーズへ
		sortieMovePhase = SortieActionPhase::RunningSideZoom;
	}
}

void SortieCamera::RunningSideZoomPlayer()
{
	//自機をズームする時間
	const float zoomTime = 240;
	//タイマー更新
	cameraActionTimer++;
	const float time = cameraActionTimer / zoomTime;

	//カメラを自機に並走させる
	eyePlayerDistance.z = Easing::OutQuint(runningSideStartDistance, runningSideEndDistance, time);
	eye = player->GetPosition() + eyePlayerDistance;

	//カメラの注視点を自機方向に移動させる
	target = player->GetPosition();
	dirtyView = true;

	//タイマーが指定した時間になったら
	if (cameraActionTimer >= zoomTime) {
		//次のフェーズへ
		sortieMovePhase = SortieActionPhase::RunningSideSpeedMove;

		//並走&ズームを終了させる
		isZoomEnd = true;
	}
}

void SortieCamera::CameraRunningSideSpeedMove()
{
	//カメラを自機の通常スピードで並走させる
	eye.z += SortiePlayer::GetAdvanceSpeed();

	//カメラの注視点を自機方向に移動させる
	const Vector3 playerPos = player->GetPosition();
	target = { playerPos.x, playerPos.y, eye.z - runningSideEndDistance };
	dirtyView = true;
}
