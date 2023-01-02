#include "Stage02GameCamera.h"
#include "Easing.h"
#include "Stage02Player.h"
#include "Boss2Body.h"

void (Stage02GameCamera::* Stage02GameCamera::stageClearCameraActionFuncTable[])() = {
	&Stage02GameCamera::StageClearBossSide,
	&Stage02GameCamera::StageClearBossBack,
	&Stage02GameCamera::StageClearPlayerFront,
	&Stage02GameCamera::StageClearPlayerAround,
	&Stage02GameCamera::StageClearPlayerBoostLookPos,
};

void Stage02GameCamera::Initialize(Stage02Player* player)
{
	//自機をセット
	this->player = player;

	//初期座標を設定
	const Vector3 playerDistance = { 0, 2, -30 };
	const Vector3 startPosition = player->GetPosition() + playerDistance;
	position = startPosition;

	//ビュー行列と射影行列の初期化
	UpdateMatView();
	UpdateMatProjection();

	//移動限界をセット
	moveLimitMin = { player->GetMoveLimitMin().x * 1.5f, player->GetMoveLimitMin().y * 1.5f };
	moveLimitMax = { player->GetMoveLimitMax().x * 1.5f, player->GetMoveLimitMax().y * 1.5f };
}

void Stage02GameCamera::Update()
{
	//カメラ動き
	CameraAction(player);

	//カメラの更新
	BaseGameCamera::Update();
}

void Stage02GameCamera::CrashStart()
{
	//墜落状態にする
	isCrash = true;
	//墜落状態のカメラ位置に移動する
	isMoveCrashPos = true;

	//墜落用の自機の中心座標からの距離
	crashPlayerDistance = { 0, 0, -player->GetBasePos().z };

	//墜落状態のカメラ位置に移動前の座標をセット
	moveCrashBeforePos = position;
	//墜落状態のカメラ位置に移動前の角度をセット
	moveCrashBeforeRota = rotation;
	//ゆらゆら角度を0にしておく
	swayX = 0;
}

void Stage02GameCamera::StageClearModeStart(Boss2Body* boss2Body)
{
	//ボス胴体をセット
	this->boss2Body = boss2Body;
	//移動前座標をセット
	stageClearMoveBeforePos = position;
	//移動前カメラ角度をセット
	stageClearMoveBeforeRota = rotation;
	//ゆらゆら角度を0にしておく
	swayX = 0;

	//ステージクリア後の動きをする
	isStageClearMode = true;
}

void Stage02GameCamera::StageClearPlayerFrontStart()
{
	//ステージクリア後の状態でなければ抜ける
	if (!isStageClearMode) { return; }

	//移動前座標をセット
	stageClearMoveBeforePos = position;
	//移動前カメラ角度をセット
	stageClearMoveBeforeRota = rotation;
	if (stageClearMoveBeforeRota.y < 0) { stageClearMoveBeforeRota.y += 360; } //角度修正

	//ステージクリア後の行動を自機の正面に移動へ
	stageClearModePhase = StageClearModePhase::PlayerFront;
	//タイマー初期化
	stageClearModeTimer = 0;
}

void Stage02GameCamera::StageClearPlayerBoostLookPosStart()
{
	//移動前の自機周りを回るときの回転角度をセット(回すために一周分の増やす)
	beforePlayerAroundRotAngle = playerAroundRotAngle - 360;
	//移動前の自機周りを回るときの自機との距離をセット
	beforePlayerAroundDistance = playerAroundDistance;
	//移動前座標をセット
	stageClearMoveBeforePos = position;

	//ステージクリア後の行動を自機のブーストを見る位置に移動へ
	stageClearModePhase = StageClearModePhase::PlayerBoostPos;
	//タイマー初期化
	stageClearModeTimer = 0;
}

void Stage02GameCamera::BossDelete()
{
	//ステージクリア後の状態でなければ抜ける
	if (!isStageClearMode) { return; }

	//ボス胴体を解除
	this->boss2Body = nullptr;
}

void Stage02GameCamera::Crash()
{
	//自機が死亡していたら抜ける
	if (player->GetIsDead()) { return; }

	//平行移動行列の計算
	XMMATRIX matTrans = XMMatrixTranslation(crashPlayerDistance.x, crashPlayerDistance.y, crashPlayerDistance.z);

	//ワールド行列の合成
	XMMATRIX bulletShotMatWorld;
	bulletShotMatWorld = XMMatrixIdentity();	//変形をリセット
	bulletShotMatWorld *= matTrans;	//ワールド行列に平行移動を反映
	//自機オブジェクトのワールド行列をかける
	bulletShotMatWorld *= player->GetMatWorld();

	//墜落用カメラ座標、回転角を取得
	const Vector3 crashCameraPos = { bulletShotMatWorld.r[3].m128_f32[0], bulletShotMatWorld.r[3].m128_f32[1], bulletShotMatWorld.r[3].m128_f32[2] };
	const Vector3 crashCameraRota = { player->GetRotation().x,  player->GetRotation().y, 0 };

	//自機の少し上にカメラを移動させる
	if (isMoveCrashPos) {
		//タイマーを更新
		const float moveCrashPosTime = 60;
		moveCrashPosTimer++;
		const float time = moveCrashPosTimer / moveCrashPosTime;

		position.x = Easing::OutQuad(moveCrashBeforePos.x, crashCameraPos.x, time);
		position.y = Easing::OutQuad(moveCrashBeforePos.y, crashCameraPos.y, time);
		position.z = Easing::OutQuad(moveCrashBeforePos.z, crashCameraPos.z, time);

		rotation.x = Easing::OutQuad(moveCrashBeforeRota.x, crashCameraRota.x, time);
		rotation.y = Easing::OutQuad(moveCrashBeforeRota.y, crashCameraRota.y, time);

		//タイマーが指定した時間になったら緊急回避終了
		if (moveCrashPosTimer >= moveCrashPosTime) {
			isMoveCrashPos = false;
		}
	}
	//移動終了後も同じ座標を追従し続ける
	else {
		//墜落用座標と回転角(自機の後ろ)を追従し続ける
		position = crashCameraPos;
		rotation.x = crashCameraRota.x;
		rotation.y = crashCameraRota.y;

		//だんだんカメラと自機の距離を離す
		crashPlayerDistance.z -= 0.1f;
	}

	//z軸をくるくる回転させておく
	const float rotZSpeed = -0.1f;
	rotation.z += rotZSpeed;
}

void Stage02GameCamera::StageClear()
{
	//ステージクリア後行動
	(this->*stageClearCameraActionFuncTable[static_cast<size_t>(stageClearModePhase)])();
}

void Stage02GameCamera::StageClearBossSide()
{
	//ボス胴体のインスタンスがなければ抜ける
	if (!boss2Body) { return; }

	//ボスの横に移動するのにかかる時間
	const float sideMoveTime = 240;
	//タイマー更新
	stageClearModeTimer++;
	//指定の時間を越えない
	stageClearModeTimer = min(stageClearModeTimer, (int)sideMoveTime);
	const float time = stageClearModeTimer / sideMoveTime;

	//座標をボス横に移動
	const Vector3 bossDistance = { 45, -1, 0 };
	const Vector3 bossSidePos = boss2Body->GetPosition() + bossDistance;
	position.x = Easing::InOutQuad(stageClearMoveBeforePos.x, bossSidePos.x, time);
	position.y = Easing::InOutQuad(stageClearMoveBeforePos.y, bossSidePos.y, time);
	position.z = Easing::InBack(stageClearMoveBeforePos.z, bossSidePos.z, time);

	//ボスの方向をだんだん向く
	const Vector3 bossCameraVec = boss2Body->GetWorldPos() - position;
	Vector3 rota = {};
	rota.y = XMConvertToDegrees(std::atan2(bossCameraVec.x, bossCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rota.y));
	Vector3 bossCameraVecZ = MatrixTransformDirection(bossCameraVec, matRot);
	rota.x = XMConvertToDegrees(std::atan2(-bossCameraVecZ.y, bossCameraVecZ.z));
	rotation.x = Easing::InQuad(stageClearMoveBeforeRota.x, rota.x, time);
	rotation.y = Easing::InQuad(stageClearMoveBeforeRota.y, rota.y, time);
	rotation.z = Easing::InQuad(stageClearMoveBeforeRota.z, rota.z, time);

	//指定した時間になったら次のフェーズへ
	if (stageClearModeTimer >= sideMoveTime) {
		stageClearModePhase = StageClearModePhase::BossBack;

		//タイマー初期化
		stageClearModeTimer = 0;
		//移動前座標をセット
		stageClearMoveBeforePos = position;
	}
}

void Stage02GameCamera::StageClearBossBack()
{
	//ボス胴体のインスタンスがなければ抜ける
	if (!boss2Body) { return; }

	//ボスの後ろに移動するのにかかる時間
	const float backMoveTime = 260;
	//タイマー更新
	stageClearModeTimer++;
	//指定の時間を越えない
	stageClearModeTimer = min(stageClearModeTimer, (int)backMoveTime);
	const float time = stageClearModeTimer / backMoveTime;

	//座標をボス後ろに移動
	const Vector3 bossDistance = { 0, -1, 80 };
	const Vector3 bossSidePos = boss2Body->GetPosition() + bossDistance;
	position.x = Easing::OutQuad(stageClearMoveBeforePos.x, bossSidePos.x, time);
	position.y = Easing::OutQuad(stageClearMoveBeforePos.y, bossSidePos.y, time);
	position.z = Easing::OutQuad(stageClearMoveBeforePos.z, bossSidePos.z, time);

	//ボスの方向を向く
	const Vector3 bossCameraVec = boss2Body->GetWorldPos() - position;
	rotation.y = XMConvertToDegrees(std::atan2(bossCameraVec.x, bossCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 bossCameraVecZ = MatrixTransformDirection(bossCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-bossCameraVecZ.y, bossCameraVecZ.z));
}

void Stage02GameCamera::StageClearPlayerFront()
{
	//自機の正面に移動するのにかかる時間
	const float playerFrontMoveTime = 240;
	//タイマー更新
	stageClearModeTimer++;
	//指定の時間を越えない
	stageClearModeTimer = min(stageClearModeTimer, (int)playerFrontMoveTime);
	const float time = stageClearModeTimer / playerFrontMoveTime;

	//自機の中心座標からの距離
	const Vector3 distancePos = { 0, -0.5f, 15.0f };
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

	//自機の方向をだんだん向く
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	Vector3 rota = {};
	rota.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	if (rota.y <= 0) { rota.y += 360; } //角度修正
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rota.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rota.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));
	rotation.x = Easing::OutCubic(stageClearMoveBeforeRota.x, rota.x, time);
	rotation.y = Easing::OutCubic(stageClearMoveBeforeRota.y, rota.y, time);
	rotation.z = Easing::OutCubic(stageClearMoveBeforeRota.z, rota.z, time);

	//指定した時間になったら次のフェーズへ
	if (stageClearModeTimer >= playerFrontMoveTime) {
		stageClearModePhase = StageClearModePhase::PlayerAround;

		//タイマー初期化
		stageClearModeTimer = 0;
		//移動前座標をセット
		stageClearMoveBeforePos = position;
		//自機との距離を記録
		playerAroundDistance = fabsf(playerCameraVec.z);
	}
}

void Stage02GameCamera::StageClearPlayerAround()
{
	//自機座標
	const Vector3 playerPos = player->GetWorldPos();

	//角度を変更し続ける
	const float rotSpeed = 0.3f;
	playerAroundRotAngle -= rotSpeed;
	//360を越えないようにする
	if (playerAroundRotAngle <= -360) { playerAroundRotAngle += 360; }

	//自機周りを回る
	const float radian = XMConvertToRadians(playerAroundRotAngle);
	position.x = playerAroundDistance * sinf(radian) + playerPos.x;
	position.z = playerAroundDistance * cosf(radian) + playerPos.z;

	//自機の方向を向く
	const Vector3 playerCameraVec = playerPos - position;
	rotation.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));
}

void Stage02GameCamera::StageClearPlayerBoostLookPos()
{
	//自機の方向を向く
	const Vector3 playerCameraVec = player->GetWorldPos() - position;
	rotation.y = XMConvertToDegrees(std::atan2(playerCameraVec.x, playerCameraVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 playerCameraVecZ = MatrixTransformDirection(playerCameraVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-playerCameraVecZ.y, playerCameraVecZ.z));

	//ステージクリア後を完了する時間
	const int stageClearModeCompletionTime = 300;
	//タイマー更新
	stageClearModeTimer++;
	//タイマーが指定した時間になったら完了フラグを立てる
	if (stageClearModeTimer > stageClearModeCompletionTime) {
		isStageClearModeCompletion = true;

		//抜ける
		return;
	}

	//自機に並走させる
	const float radian = XMConvertToRadians(playerAroundRotAngle);
	const Vector3 playerPos = player->GetWorldPos();//自機座標
	position.x = playerAroundDistance * sinf(radian) + playerPos.x;
	position.z = playerAroundDistance * cosf(radian) + playerPos.z;

	//自機周り1周にかかる時間
	const float rotRoundTime = 240;
	if (stageClearModeTimer > rotRoundTime) { return; }
	const float time = stageClearModeTimer / rotRoundTime;

	//自機の周りを一周させる
	playerAroundRotAngle = Easing::OutQuart(beforePlayerAroundRotAngle, -165, time);
	playerAroundDistance = Easing::OutQuart(beforePlayerAroundDistance, 17.5f, time);
	position.y = Easing::OutQuart(stageClearMoveBeforePos.y, playerPos.y + 6, time);
}
