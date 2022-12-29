#include "Stage01GameCamera.h"
#include "Easing.h"
#include "Stage01Player.h"
#include "BossMainBody.h"

void (Stage01GameCamera::* Stage01GameCamera::stageClearCameraActionFuncTable[])() = {
	&Stage01GameCamera::StageClearBossLook,
	&Stage01GameCamera::StageClearFrontLook,
	&Stage01GameCamera::StageClearPlayerLook,
	&Stage01GameCamera::StageClearPlayerZoom,
	&Stage01GameCamera::StageClearPlayerFollow,
	&Stage01GameCamera::StageClearPlayerSideMove,
	&Stage01GameCamera::StageClearPlayerKeepLook,
};

void Stage01GameCamera::Initialize(Stage01Player* player)
{
	//自機をセット
	this->player = player;

	//初期座標を設定
	const Vector3 playerPos = player->GetPosition();
	const Vector3 startPosition = { playerPos.x, playerPos.y + 10, -30 };
	position = startPosition;

	//ビュー行列と射影行列の初期化
	UpdateMatView();
	UpdateMatProjection();

	//移動限界をセット
	moveLimitMin = { player->GetMoveLimitMin().x * 1.5f, 8.0f };
	moveLimitMax = { player->GetMoveLimitMax().x * 1.5f, moveLimitMin.y + (player->GetMoveLimitMax().y - player->GetMoveLimitMin().y) * 1.5f };
}

void Stage01GameCamera::Update()
{
	//カメラ動き
	CameraAction(player);

	//カメラの更新
	BaseGameCamera::Update();
}

void Stage01GameCamera::CrashStart()
{
	//墜落状態にする
	isCrash = true;
	//墜落状態のカメラ位置に移動する
	isMoveCrashPos = true;

	//墜落状態のカメラ位置に移動前の座標をセット
	moveCrashBeforePos = position;
	//ゆらゆら角度を0にしておく
	swayX = 0;
}

void Stage01GameCamera::StageClearModeStart(BossMainBody* bossMainBody)
{
	//ボス本体をセット
	this->bossMainBody = bossMainBody;
	//ボスの方向を向くときのY座標をセット
	const float upperNum = 10.0f;
	bossLookPosY = bossMainBody->GetWorldPos().y + upperNum;
	//移動前座標をセット
	stageClearMoveBeforePos = position;
	//移動前カメラ角度をセット
	stageClearMoveBeforeRota = rotation;
	//ゆらゆら角度を0にしておく
	swayX = 0;

	//ステージクリア後の動きをする
	isStageClearMode = true;
}

void Stage01GameCamera::BossDelete()
{
	//ステージクリア後の状態でなければ抜ける
	if (!isStageClearMode) { return; }

	//ボス本体を解除
	this->bossMainBody = nullptr;
	//移動前カメラ角度をセット
	stageClearMoveBeforeRota = rotation;
	//ステージクリア後の挙動を正面を向くへ
	stageClearModePhase = StageClearModePhase::FrontLook;
	//タイマー初期化
	stageClearModeTimer = 0;
}

void Stage01GameCamera::Crash()
{
	//自機の少し上にカメラを移動させる
	const Vector3 crashCameraPos = { player->GetWorldPos().x, player->GetWorldPos().y + 3, 0 };
	if (isMoveCrashPos) {
		//タイマーを更新
		const float moveCrashPosTime = 40;
		moveCrashPosTimer++;
		const float time = moveCrashPosTimer / moveCrashPosTime;

		position.x = Easing::OutQuad(moveCrashBeforePos.x, crashCameraPos.x, time);
		position.y = Easing::OutQuad(moveCrashBeforePos.y, crashCameraPos.y, time);

		//タイマーが指定した時間になったら緊急回避終了
		if (moveCrashPosTimer >= moveCrashPosTime) {
			isMoveCrashPos = false;
		}
	}
	//移動終了後も同じ座標を追従し続ける
	else {
		//X,Y座標は自機の少し上を追従する
		position.x = crashCameraPos.x;
		position.y = crashCameraPos.y;
	}

	//Z軸回転する
	const float rotSpeed = 1.2f;
	//自機が2回バウンドするまでZ軸回転する
	if (player->GetCrashBoundCount() == 0) { rotation.z += rotSpeed; }
	else if (player->GetCrashBoundCount() == 1) { rotation.z -= rotSpeed; }
}

void Stage01GameCamera::StageClear()
{
	//ステージクリア後行動
	(this->*stageClearCameraActionFuncTable[static_cast<size_t>(stageClearModePhase)])();
}

void Stage01GameCamera::StageClearBossLook()
{
	//ボス本体のインスタンスがなければ抜ける
	if (!bossMainBody) { return; }

	//ボスの方向を向く時間
	const float bossLookTime = 120;
	//タイマー更新
	stageClearModeTimer++;
	//指定の時間を越えない
	stageClearModeTimer = min(stageClearModeTimer, (int)bossLookTime);
	const float time = stageClearModeTimer / bossLookTime;

	//座標を上に移動
	position.y = Easing::OutCubic(stageClearMoveBeforePos.y, bossLookPosY, time);

	//X座標が中心付近にいたら左右にずらす
	const float xPosMin = 10.0f;
	if (fabsf(stageClearMoveBeforePos.x) <= xPosMin) {
		if (stageClearMoveBeforePos.x >= 0) { position.x = Easing::OutCubic(stageClearMoveBeforePos.x, xPosMin, time); }
		else { position.x = Easing::OutCubic(stageClearMoveBeforePos.x, -xPosMin, time); }
	}

	//ボスの方向をだんだん向く
	const Vector3 bossCameraVec = bossMainBody->GetWorldPos() - position;
	Vector3 rota = {};
	rota.y = XMConvertToDegrees(std::atan2(bossCameraVec.x, bossCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rota.y));
	Vector3 bossCameraVecZ = MatrixTransformDirection(bossCameraVec, matRot);
	rota.x = XMConvertToDegrees(std::atan2(-bossCameraVecZ.y, bossCameraVecZ.z));
	rotation.x = Easing::OutCubic(stageClearMoveBeforeRota.x, rota.x, time);
	rotation.y = Easing::OutCubic(stageClearMoveBeforeRota.y, rota.y, time);
	rotation.z = Easing::OutCubic(stageClearMoveBeforeRota.z, rota.z, time);
}

void Stage01GameCamera::StageClearFrontLook()
{
	//方向を切り替える時間
	const float lookChangeTime = 300;
	//タイマー更新
	stageClearModeTimer++;
	stageClearModeTimer = min(stageClearModeTimer, (int)lookChangeTime);
	const float time = stageClearModeTimer / lookChangeTime;

	//正面を向く
	rotation.x = Easing::InOutCubic(stageClearMoveBeforeRota.x, 0, time);
	rotation.y = Easing::InOutCubic(stageClearMoveBeforeRota.y, 0, time);
	rotation.z = Easing::InOutCubic(stageClearMoveBeforeRota.z, 0, time);

	//自機がカメラの後ろにいったら次のフェーズへ
	const float changePhaseDistance = 10.0f;
	if (player->GetWorldPos().z <= position.z + changePhaseDistance) {
		stageClearModePhase = StageClearModePhase::PlayerLook;

		//移動前カメラ角度をセット
		stageClearMoveBeforeRota = rotation;
		//タイマー初期化
		stageClearModeTimer = 0;
	}
}

void Stage01GameCamera::StageClearPlayerLook()
{
	//方向を切り替える時間
	const float lookChangeTime = 20;
	//タイマー更新
	stageClearModeTimer++;
	//指定の時間を越えない
	stageClearModeTimer = min(stageClearModeTimer, (int)lookChangeTime);
	const float time = stageClearModeTimer / lookChangeTime;

	//自機の方向をだんだん向く
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	Vector3 rota = {};
	rota.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rota.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rota.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));
	rotation.x = Easing::InOutCubic(stageClearMoveBeforeRota.x, rota.x, time);
	rotation.y = Easing::InOutCubic(stageClearMoveBeforeRota.y, rota.y, time);
	rotation.z = Easing::InOutCubic(stageClearMoveBeforeRota.z, 0, time);

	//自機が停止状態に入ったら次のフェーズへ
	if (player->GetStageClearModePhase() == Stage01Player::StageClearModePhase::Stay) {
		stageClearModePhase = StageClearModePhase::PlayerZoom;

		//タイマー初期化
		stageClearModeTimer = 0;
		//移動前座標をセット
		stageClearMoveBeforePos = position;
	}
}

void Stage01GameCamera::StageClearPlayerZoom()
{
	//ズームする時間
	const float zoomTime = 300;
	//タイマー更新
	stageClearModeTimer++;
	const float time = stageClearModeTimer / zoomTime;

	//自機の中心座標からの距離
	const Vector3 distancePos = { 0, 0.5f, -10.0f };
	//平行移動行列の計算
	XMMATRIX matTrans = XMMatrixTranslation(distancePos.x, distancePos.y, distancePos.z);
	//自機の後ろワールド行列
	XMMATRIX playerBackMatWorld;
	playerBackMatWorld = XMMatrixIdentity();	//変形をリセット
	playerBackMatWorld *= matTrans;	//ワールド行列に平行移動を反映
	//自機のワールド行列をかける
	playerBackMatWorld *= player->GetMatWorld();
	//自機の後ろ座標を取得
	Vector3 playerBack = { playerBackMatWorld.r[3].m128_f32[0], playerBackMatWorld.r[3].m128_f32[1], playerBackMatWorld.r[3].m128_f32[2] };

	//自機の後ろにズームする
	position.x = Easing::InOutQuad(stageClearMoveBeforePos.x, playerBack.x, time);
	position.y = Easing::InOutQuad(stageClearMoveBeforePos.y, playerBack.y, time);
	position.z = Easing::InOutQuad(stageClearMoveBeforePos.z, playerBack.z, time);

	//自機の方向を向く
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	rotation.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));

	//指定した時間になったら次のフェーズへ
	if (stageClearModeTimer >= zoomTime) {
		stageClearModePhase = StageClearModePhase::PlayerFollow;

		//タイマー初期化
		stageClearModeTimer = 0;
	}
}

void Stage01GameCamera::StageClearPlayerFollow()
{
	//追従する時間
	const float followTime = 90;
	//タイマー更新
	stageClearModeTimer++;
	const float time = stageClearModeTimer / followTime;

	//指定した時間になったら次のフェーズへ
	if (stageClearModeTimer >= followTime) {
		stageClearModePhase = StageClearModePhase::PlayerSideMove;

		//タイマー初期化
		stageClearModeTimer = 0;

		//移動前座標をセット
		stageClearMoveBeforePos = position;
	}
}

void Stage01GameCamera::StageClearPlayerSideMove()
{
	//移動する時間
	const float moveTime = 300;

	//自機の方向を向く
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	rotation.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));

	//タイマー更新
	stageClearModeTimer++;
	float time = stageClearModeTimer / moveTime;

	//自機の中心座標からの距離
	const Vector3 distancePos = { 10.0f, 0, -4.0f };
	//平行移動行列の計算
	XMMATRIX matTrans = XMMatrixTranslation(distancePos.x, distancePos.y, distancePos.z);
	//自機横ワールド行列
	XMMATRIX playerSideMatWorld;
	playerSideMatWorld = XMMatrixIdentity();	//変形をリセット
	playerSideMatWorld *= matTrans;	//ワールド行列に平行移動を反映
	//自機のワールド行列をかける
	playerSideMatWorld *= player->GetMatWorld();
	//自機横座標を取得(Y座標は移動前より少し上)
	const float upNum = 0.5f;
	Vector3 playerSide = { playerSideMatWorld.r[3].m128_f32[0], stageClearMoveBeforePos.y + upNum, playerSideMatWorld.r[3].m128_f32[2] };

	//自機横に移動する
	position.x = Easing::OutQuad(stageClearMoveBeforePos.x, playerSide.x, time);
	position.y = Easing::OutQuad(stageClearMoveBeforePos.y, playerSide.y, time);
	position.z = Easing::OutQuad(stageClearMoveBeforePos.z, playerSide.z, time);

	//指定した時間になったら次のフェーズへ
	if (stageClearModeTimer >= moveTime) {
		stageClearModePhase = StageClearModePhase::PlayerKeepLook;
	}
}

void Stage01GameCamera::StageClearPlayerKeepLook()
{
	//自機の方向を向く
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	rotation.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));
}
