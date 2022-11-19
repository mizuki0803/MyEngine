#include "GameCamera.h"
#include "Input.h"
#include "Easing.h"
#include "Player.h"
#include "BossMainBody.h"

void (GameCamera::* GameCamera::stageClearCameraActionFuncTable[])() = {
	&GameCamera::StageClearBossLook,
	&GameCamera::StageClearFrontLook,
	&GameCamera::StageClearPlayerLook,
	&GameCamera::StageClearPlayerZoom,
	&GameCamera::StageClearPlayerFollow,
	&GameCamera::StageClearPlayerSideMove,
	&GameCamera::StageClearPlayerKeepLook,
};

const float GameCamera::advanceSpeed = 0.3f;

void GameCamera::Initialize()
{
	//初期座標を設定
	position = { 0, 13, -30 };
	//初期回転角を設定
	rotation = { 0, 0, 0 };

	//ビュー行列と射影行列の初期化
	UpdateMatView();
	UpdateMatProjection();
}

void GameCamera::Update()
{
	//カメラ動き
	CameraAction();

	//回転　平行移動行列の計算
	DirectX::XMMATRIX matRot, matTrans;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//ワールド行列の合成
	matWorld = XMMatrixIdentity();	//変形をリセット
	matWorld *= matRot;		//ワールド行列に回転を反映
	matWorld *= matTrans;	//ワールド行列に平行移動を反映

	//視点をワールド座標に設定
	eye = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//カメラの回転を反映させる
	forward = MatrixTransformDirection(forward, matWorld);
	//視点から前方に進んだ位置を注視点に設定
	target = eye + forward;
	//天地が反転してもいいように上方向ベクトルも回転させる
	Vector3 baseUp(0, 1, 0);
	up = MatrixTransformDirection(baseUp, matWorld);

	//シェイク状態ならカメラをシェイクさせる
	if (isShake) {
		Shake();
	}

	//ビュー行列と射影行列の更新
	UpdateMatView();
	UpdateMatProjection();
}

void GameCamera::CrashStart()
{
	//墜落状態にする
	isCrash = true;
	//墜落状態のカメラ位置に移動する
	isMoveCrashPos = true;

	//墜落状態のカメラ位置に移動前の座標をセット
	moveCrashBeforePos = position;
}

void GameCamera::ShakeStart()
{
	//シェイクタイマーをリセット
	shakeTimer = 0;
	//シェイク状態にする
	isShake = true;
}

void GameCamera::StageClearModeStart(BossMainBody* bossMainBody)
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

	//ステージクリア後の動きをする
	isStageClearMode = true;
}

void GameCamera::BossDelete()
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

void GameCamera::CameraAction()
{
	//ステージクリア状態
	if (isStageClearMode) {
		//ステージクリア後行動
		(this->*stageClearCameraActionFuncTable[static_cast<size_t>(stageClearModePhase)])();
	}
	//墜落状態
	else if (isCrash) {
		Crash();
	}
	//それ以外
	else {
		//回転
		Rotate();

		//プレイヤーがダメージ状態ならノックバックする
		if (player->GetIsDamage()) {
			Knockback();
		}
		//ダメージ状態でないなら通常移動
		else {
			Move();
		}
	}
}

void GameCamera::Crash()
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
	const float rotSpeed = 1.5f;
	//自機が一回バウンドするまではZ方向に移動する
	if (player->GetCrashBoundCount() == 0) {
		rotation.z += rotSpeed;
		position.z += 0.1f;
	}
	else if (player->GetCrashBoundCount() == 1) { rotation.z -= rotSpeed; }
}

void GameCamera::Rotate()
{
	//回転(レールカメラに追従している自機の傾きを利用する)
	rotation.x = player->GetRotation().x / 5;
	rotation.y = player->GetRotation().y / 5;
	rotation.z = -player->GetRotation().y / 8;
}

void GameCamera::Move()
{
	//移動速度
	Vector3 velocity;
	//カメラが傾いている角度に移動させる
	const float moveSpeed = Player::GetMoveBaseSpeed() * 8;
	const Vector2 rotLimit = Player::GetRotLimit();
	velocity.x = moveSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveSpeed * -(rotation.x / rotLimit.x);

	//前進する場合はZ方向に移動
	if (isAdvance) { velocity.z = advanceSpeed; }
	//移動
	position += velocity;

	//移動限界から出ないようにする
	const Vector2 moveLimitMin = { Player::GetMoveLimitMin().x * 1.5f, 8.0f };
	const Vector2 moveLimitMax = { Player::GetMoveLimitMax().x * 1.5f, moveLimitMin.y + (Player::GetMoveLimitMax().y - Player::GetMoveLimitMin().y) * 1.5f };
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void GameCamera::Knockback()
{
	//自機にあわせてカメラをノックバックさせる
	const float speed = Player::GetKnockbackBaseSpeed() * 8;
	Vector3 velocity = player->GetKnockbackVel();
	velocity *= speed;

	//前進する場合はZ方向に移動
	if (isAdvance) { velocity.z = advanceSpeed; }
	//移動
	position += velocity;

	//移動限界から出ないようにする
	const Vector2 moveLimitMin = { Player::GetMoveLimitMin().x * 1.5f, 8.0f };
	const Vector2 moveLimitMax = { Player::GetMoveLimitMax().x * 1.5f, moveLimitMin.y + (Player::GetMoveLimitMax().y - Player::GetMoveLimitMin().y) * 1.5f };
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void GameCamera::Shake()
{
	//シェイクする時間を設定
	const float shakeTime = 20;
	//タイマーをカウント
	shakeTimer++;
	const float time = shakeTimer / shakeTime;
	//シェイクの最大の強さを設定
	const float maxShakePower = 15;

	//シェイクする値を計算
	const float randShake = maxShakePower * (1 - time);
	Vector3 shake{};

	//ゼロ除算を避けるために0の場合はランダムを生成しない
	if (!((int)randShake == 0)) {
		shake.x = (float)((rand() % (int)randShake) - randShake / 2);
		shake.y = (float)((rand() % (int)randShake) - randShake / 2);
	}

	//値が大きいので割り算して小さくする
	const float div = 100;
	shake /= div;

	//カメラにシェイクの値を加算
	eye += shake;

	//シェイクが完了したらシェイク状態を解除
	if (shakeTimer >= shakeTime) {
		isShake = false;
	}
}

void GameCamera::StageClearBossLook()
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

void GameCamera::StageClearFrontLook()
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

void GameCamera::StageClearPlayerLook()
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
	if (player->GetStageClearModePhase() == Player::StageClearModePhase::Stay) {
		stageClearModePhase = StageClearModePhase::PlayerZoom;

		//タイマー初期化
		stageClearModeTimer = 0;
		//移動前座標をセット
		stageClearMoveBeforePos = position;
	}
}

void GameCamera::StageClearPlayerZoom()
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

void GameCamera::StageClearPlayerFollow()
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

void GameCamera::StageClearPlayerSideMove()
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

void GameCamera::StageClearPlayerKeepLook()
{
	//自機の方向を向く
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	rotation.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));
}
