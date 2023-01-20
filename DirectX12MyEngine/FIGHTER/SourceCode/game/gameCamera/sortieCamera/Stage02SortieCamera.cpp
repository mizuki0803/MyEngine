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
	//カメラ初期化
	Camera::Initialize();

	//カメラの初期値をセット
	const Vector3 startEyePos = { 35, 1, 0 };
	eye = startEyePos;
	moveBeforeEye = eye;
}

void Stage02SortieCamera::Update()
{
	//行動
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieMovePhase)])();

	//カメラ更新
	Camera::Update();
}

void Stage02SortieCamera::LookPlayer()
{
	//自機の方向を向くのにかかる時間
	const float lookTime = 400;
	//タイマー更新
	cameraActionTimer++;
	const float time = cameraActionTimer / lookTime;

	//カメラの注視点を自機方向に移動させる
	const Vector3 startTargetPos = { 0, 1, eye.z + 60 };
	const Vector3 playerPos = player->GetPosition();
	target.x = Easing::InOutQuad(startTargetPos.x, playerPos.x, time);
	target.y = Easing::InOutQuad(startTargetPos.y, playerPos.y, time);
	target.z = Easing::InOutQuad(startTargetPos.z, playerPos.z, time);
	dirtyView = true;

	//タイマーが指定した時間になったら
	if (cameraActionTimer >= lookTime) {
		//次のフェーズへ
		sortieMovePhase = Stage02SortieActionPhase::ZoomPlayer;

		//タイマーを初期化
		cameraActionTimer = 0;
	}
}

void Stage02SortieCamera::ZoomPlayer()
{
	//ズームにかかる時間
	const float zoomTime = 180;
	//タイマー更新
	cameraActionTimer++;
	const float time = cameraActionTimer / zoomTime;

	//カメラを自機にズームさせる
	const Vector3 playerPos = player->GetPosition();
	const Vector3 moveAfterPos = playerPos + playerDistance;
	eye.x = Easing::OutQuad(moveBeforeEye.x, moveAfterPos.x, time);
	eye.y = Easing::OutQuad(moveBeforeEye.y, moveAfterPos.y, time);
	eye.z = Easing::OutQuad(moveBeforeEye.z, moveAfterPos.z, time);
	target = playerPos;
	dirtyView = true;

	//タイマーが指定した時間になったら
	if (cameraActionTimer >= zoomTime) {
		//次のフェーズへ
		sortieMovePhase = Stage02SortieActionPhase::RunningSide;

		//タイマーを初期化
		cameraActionTimer = 0;
	}
}

void Stage02SortieCamera::RunningSidePlayer()
{
	//並走時間
	const float runningTime = 215;
	//タイマー更新
	cameraActionTimer++;

	//カメラを自機に並走させる
	const Vector3 playerPos = player->GetPosition();
	eye = playerPos + playerDistance;
	target = playerPos;
	dirtyView = true;

	//タイマーが指定した時間になったら
	if (cameraActionTimer >= runningTime) {
		//次のフェーズへ
		sortieMovePhase = Stage02SortieActionPhase::Stay;

		//並走&ズームを終了させる
		isZoomEnd = true;
	}
}

void Stage02SortieCamera::Stay()
{
}
