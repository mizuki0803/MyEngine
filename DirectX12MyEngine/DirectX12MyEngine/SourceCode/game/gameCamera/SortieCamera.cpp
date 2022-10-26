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
	//カメラ初期化
	Camera::Initialize();

	//カメラの視点を自機の後方左上にセット
	eyePlayerDistance = { 0, 2, 400 };
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
	const float zoomTime = 300;
	//タイマー更新
	cameraActionTimer++;
	const float time = cameraActionTimer / zoomTime;

	//並走終了時の自機とカメラの距離
	const float runningSideEndDistance = 15.0f;
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
	//並走終了時の自機とカメラの距離
	const float runningSideEndDistance = 15.0f;
	//カメラを自機に並走させる
	eye.z += 1.0f;

	//カメラの注視点を自機方向に移動させる
	const Vector3 playerPos = player->GetPosition();
	target = { playerPos.x, playerPos.y, eye.z - runningSideEndDistance };
	
	dirtyView = true;
}
