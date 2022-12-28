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
	//カメラ初期化
	Camera::Initialize();

	//カメラの視点を自機の後方左上にセット
	eyePlayerDistance = { 0, 1.5f, 350 };
	eye = player->GetPosition() + eyePlayerDistance;
}

void Stage01SortieCamera::Update()
{
	//行動
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieMovePhase)])();

	//カメラ更新
	Camera::Update();
}

void Stage01SortieCamera::StayPlayer()
{
	//カメラの注視点を自機方向に移動させる
	target = player->GetPosition();
	dirtyView = true;

	//自機との距離が並走を開始する距離になったら
	eyePlayerDistance.z = eye.z - player->GetPosition().z;
	if (eyePlayerDistance.z <= runningSideStartDistance) {
		//次のフェーズへ
		sortieMovePhase = Stage01SortieActionPhase::RunningSideZoom;
	}
}

void Stage01SortieCamera::RunningSideZoomPlayer()
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
		sortieMovePhase = Stage01SortieActionPhase::RunningSideSpeedMove;

		//並走&ズームを終了させる
		isZoomEnd = true;
	}
}

void Stage01SortieCamera::CameraRunningSideSpeedMove()
{
	//カメラを自機の通常スピードで並走させる
	eye.z += Stage01SortiePlayer::GetAdvanceSpeed();

	//カメラの注視点を自機方向に移動させる
	const Vector3 playerPos = player->GetPosition();
	target = { playerPos.x, playerPos.y, eye.z - runningSideEndDistance };
	dirtyView = true;
}
