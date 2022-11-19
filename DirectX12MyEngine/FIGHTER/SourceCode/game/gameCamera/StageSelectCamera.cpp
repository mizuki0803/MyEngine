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
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//カメラ更新
	Camera::Update();
}

void StageSelectCamera::GooutPlayerLookActionStart(const Vector3& gooutPlanetPos, const Vector3& selectFieldPos)
{
	//タイマー初期化
	cameraActionTimer = 0;
	//自機が出てくる惑星の座標を記憶
	this->gooutPlanetPos = gooutPlanetPos;

	//カメラ視点を自機が出てくる惑星のフィールドからカメラの距離に設定
	eye = selectFieldPos + gooutPlayerLookSelectFieldDistance;

	//カメラの注視点を出てくる惑星に設定
	target = gooutPlanetPos;

	//惑星から出てくる自機を見る状態に変更
	phase = ActionPhase::GooutPlayerLook;
}

void StageSelectCamera::StageSelectStartActionStart()
{
	//タイマー初期化
	cameraActionTimer = 0;

	//移動前の視点と注視点をセット
	moveBeforeEye = eye;
	moveBeforeTarget = target;

	//ステージ選択開始状態に変更
	phase = ActionPhase::StageSelectStart;
}

void StageSelectCamera::StageDecisionCheckActionStart()
{
	//タイマー初期化
	cameraActionTimer = 0;

	//ステージ決定確認状態に変更
	phase = ActionPhase::StageDecisionCheck;
}

void StageSelectCamera::ReturnSelectActionStart()
{
	//タイマー初期化
	cameraActionTimer = 0;

	//ステージ選択に戻る状態に変更
	phase = ActionPhase::ReturnSelect;
}

void StageSelectCamera::GooutPlayerLookAction()
{
}

void StageSelectCamera::StageSelectStartAction()
{
	//カメラ移動させる時間
	const float cameraMoveTime = 60;
	//タイマー更新
	cameraActionTimer++;
	const float time = cameraActionTimer / cameraMoveTime;

	//カメラ視点をイージングで動かす
	const Vector3 afterEye = player->GetPosition() + stageSelectPlayerDistance;
	eye.x = Easing::OutQuad(moveBeforeEye.x, afterEye.x, time);
	eye.y = Easing::OutQuad(moveBeforeEye.y, afterEye.y, time);
	eye.z = Easing::OutQuad(moveBeforeEye.z, afterEye.z, time);
	//カメラ注視点をイージングで動かす
	const Vector3 afterTarget = player->GetPosition();
	target.x = Easing::OutQuad(moveBeforeTarget.x, afterTarget.x, time);
	target.y = Easing::OutQuad(moveBeforeTarget.y, afterTarget.y, time);
	target.z = Easing::OutQuad(moveBeforeTarget.z, afterTarget.z, time);

	dirtyView = true;

	//タイマーが指定した時間になったら
	if (cameraActionTimer >= cameraMoveTime) {
		//ステージ選択状態に変更
		phase = ActionPhase::StageSelect;
	}
}

void StageSelectCamera::StageSelectAction()
{
	//カメラ視点をステージ選択状態で並走する自機とカメラの距離に設定
	eye = player->GetPosition() + stageSelectPlayerDistance;

	//カメラの注視点を自機に設定
	target = player->GetPosition();
	dirtyView = true;
}

void StageSelectCamera::StageDecisionCheckAction()
{
	//カメラ移動させる時間
	const float cameraMoveTime = 40;

	//タイマーが指定した時間以上なら抜ける
	if (cameraActionTimer >= cameraMoveTime) { return; }
	//タイマー更新
	cameraActionTimer++;
	const float time = cameraActionTimer / cameraMoveTime;

	//カメラ視点をステージ決定確認状態の自機とカメラの距離に動かす
	Vector3 playerDistance;
	playerDistance.x = Easing::OutQuad(stageSelectPlayerDistance.x, stageDecisionCheckPlayerDistance.x, time);
	playerDistance.y = Easing::OutQuad(stageSelectPlayerDistance.y, stageDecisionCheckPlayerDistance.y, time);
	playerDistance.z = Easing::OutQuad(stageSelectPlayerDistance.z, stageDecisionCheckPlayerDistance.z, time);
	eye = player->GetPosition() + playerDistance;

	//カメラの注視点を自機に設定
	target = player->GetPosition();
	dirtyView = true;
}

void StageSelectCamera::ReturnSelectAction()
{
	//カメラ移動させる時間
	const float cameraMoveTime = 40;
	//タイマー更新
	cameraActionTimer++;
	const float time = cameraActionTimer / cameraMoveTime;

	//カメラ視点をステージ選択状態の自機とカメラの距離に動かす
	Vector3 playerDistance;
	playerDistance.x = Easing::OutQuad(stageDecisionCheckPlayerDistance.x, stageSelectPlayerDistance.x, time);
	playerDistance.y = Easing::OutQuad(stageDecisionCheckPlayerDistance.y, stageSelectPlayerDistance.y, time);
	playerDistance.z = Easing::OutQuad(stageDecisionCheckPlayerDistance.z, stageSelectPlayerDistance.z, time);
	eye = player->GetPosition() + playerDistance;

	//カメラの注視点を自機に設定
	target = player->GetPosition();
	dirtyView = true;

	//タイマーが指定した時間になったら
	if (cameraActionTimer >= cameraMoveTime) {
		//ステージ選択状態に変更
		phase = ActionPhase::StageSelect;
	}
}
