#include "Stage01Player.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (Stage01Player::* Stage01Player::stageClearActionFuncTable[])() = {
	&Stage01Player::StageClearSideMove,
	&Stage01Player::StageClearReturn,
	&Stage01Player::StageClearUp,
	&Stage01Player::StageClearStay,
	&Stage01Player::StageClearBoost,
};

Stage01Player* Stage01Player::Create(ObjModel* model, const int startHP, const int maxHP)
{
	//ステージ01自機のインスタンスを生成
	Stage01Player* player = new Stage01Player();
	if (player == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!player->Initialize(model, startHP, maxHP)) {
		delete player;
		assert(0);
		return nullptr;
	}

	return player;
}

bool Stage01Player::Initialize(ObjModel* model, const int startHP, const int maxHP)
{
	//基準の座標をセット
	basePos = { 0, 3, 16 };

	//自機の共通初期化
	if (!BasePlayer::Initialize(model, startHP, maxHP)) {
		return false;
	}

	//自機の移動限界をセット
	moveLimitMin = { -15.0f, -4.0f };
	moveLimitMax = { 15.0f, moveLimitMin.y + 12.0f };

	return true;
}

void Stage01Player::StageClearModeStart()
{
	//カメラ追従を解除
	SetIsCameraFollow(false);
	//カメラ追従解除により、ローカル座標にワールド座標を代入
	position = GetWorldPos();

	//ステージクリア移動を右か左か決める
	if (position.x < 0) { isStageClearMoveRight = false; }
	else { isStageClearMoveRight = true; }

	//初期速度をセット
	stageClearMoveVelocity = { 0.2f, 0, 0.2f };
	//左側移動時のためマイナスを渡す
	if (!isStageClearMoveRight) {
		stageClearMoveVelocity.x = -stageClearMoveVelocity.x;
	}
	//回転速度をセット
	stageClearRota = rotation;

	//パーティクルの大きさを統一するため、移動はもうしないが通常移動状態にしておく
	moveSpeedPhase = MoveSpeedPhase::NormalSpeed;

	//ステージクリア後の動きをする
	isStageClearMode = true;
}

void Stage01Player::StageClearReturnStart(const Vector3& cameraPos)
{
	//カメラ座標を取得
	stageClearCameraPos = cameraPos;

	//カメラホーミング用座標をセット
	cameraHomingPos = cameraPos;
	cameraHomingPos.y += 2.0f;

	//初期速度をセット
	stageClearMoveVelocity = { 0, 0, 2.0f };
	//帰還状態にする
	stageClearModePhase = StageClearModePhase::Return;

	//タイマー初期化
	stageClearModeTimer = 0;
}

void Stage01Player::StageClearBoostStart()
{
	//ブースト状態にする
	stageClearModePhase = StageClearModePhase::Boost;
}

void Stage01Player::Crash()
{
	//座標移動
	//墜落加速度
	Vector3 crashAccel = { 0, -0.01f, 0 };
	crashVel += crashAccel;
	position += crashVel;

	//墜落回転する
	const Vector3 rotSpeed = { 0, 0.1f, 7 };
	//バウンドするまではZ軸左回転
	if (crashBoundCount == 0) { rotation.z += rotSpeed.z; }
	//1回バウンドしたらZ軸右回転 & Y軸回転
	else if (crashBoundCount == 1) { rotation -= rotSpeed; }

	//ブラーの強さを通常移動時の強さに戻していく
	SpeedChangeNormalBlur();

	//Y座標が0以下でなければ抜ける
	if (!(position.y <= 0)) { return; }
	//座標は0以下にならない
	position.y = 0;

	//墜落バウンド回数を増加させる
	crashBoundCount++;

	//爆発演出用変数
	float explosionSize = 0;
	int explosionTime = 0;

	//墜落バウンド回数が一回目ならバウンドさせる
	if (crashBoundCount == 1) {
		//バウンド
		const float boundStartVel = fabsf(crashVel.y) * 0.7f;
		crashVel.y = boundStartVel;

		//バウンド用爆発演出用設定
		explosionSize = 0.5f;
		explosionTime = 20;
	}
	//墜落バウンド回数が二回目なら死亡
	else if (crashBoundCount >= 2) {
		isDead = true;

		//死亡用爆発演出用設定
		explosionSize = 3.5f;
		explosionTime = 60;
	}
	//爆発演出用パーティクル生成
	ParticleEmitter::GetInstance()->Explosion(position, explosionSize, explosionTime);
}

void Stage01Player::Rotate()
{
	//自機回転処理
	BasePlayer::Rotate();

	//地面に自機がめり込むワールドY座標
	const float groundPos = 8.0f;
	//地面にめり込まないようにする場合の角度限界値処理
	if (GetWorldPos().y <= groundPos) {
		//ワールドY座標の限界値
		const float moveLimitWorldY = 4.0f;
		//地面から角度制限開始座標までの割合を計算
		const float ratio = (GetWorldPos().y - moveLimitWorldY) / (groundPos - moveLimitWorldY);

		//割合値が0以上の場合
		if (ratio >= 0) {
			//地面に近づくほど傾かないようにする(通常の角度制限値 * 地面に近い割合)
			const float groundRotLimit = rotLimit.x * ratio;
			rotation.x = min(rotation.x, groundRotLimit);
		}
		//0未満の場合
		else {
			//下方向に傾かないようにする
			rotation.x = min(rotation.x, 0);
		}
	}
	//地面にめり込むワールドY座標と関係ない場所にいる場合は通常通りの角度制限を行う
	else {
		rotation.x = min(rotation.x, +rotLimit.x);
	}
}

void Stage01Player::StageClear()
{
	//ステージクリア後行動
	(this->*stageClearActionFuncTable[static_cast<size_t>(stageClearModePhase)])();
}

void Stage01Player::StageClearSideMove()
{
	//タイマー更新
	stageClearModeTimer++;

	//加速度を設定
	Vector3 accel = { 0.003f, 0, 0.002f };
	//回転角度をセット
	float maxRotZ = -(360 + 65);
	//左側移動時の設定
	if (!isStageClearMoveRight) { accel.x = -accel.x;  maxRotZ = -maxRotZ; }

	//速度に加速度を加算
	stageClearMoveVelocity += accel;
	//座標を移動
	position += stageClearMoveVelocity;

	//回転
	const float rotTime = 120;
	float time = stageClearModeTimer / rotTime;
	time = min(time, 1);
	rotation.x = Easing::OutQuart(stageClearRota.x, 0, time);
	rotation.y = Easing::OutQuart(stageClearRota.y, 0, time);
	rotation.z = Easing::OutQuart(stageClearRota.z, maxRotZ, time);

	//ブラーの強さを通常移動時の強さに戻していく
	SpeedChangeNormalBlur();
}

void Stage01Player::StageClearReturn()
{
	//タイマー更新
	const float maxTime = 250;
	stageClearModeTimer++;
	const float time = stageClearModeTimer / maxTime;

	//指定した時間を過ぎたら
	float homingDistance = 100;
	const float distanceChangeStartTime = 120;
	if (stageClearModeTimer >= distanceChangeStartTime) {
		//ホーミング方向位置を奥からずらす
		const float changeTime = (stageClearModeTimer - distanceChangeStartTime) / (maxTime - distanceChangeStartTime);
		homingDistance = Easing::LerpFloat(100, 0, changeTime);
	}
	cameraHomingPos.z = stageClearCameraPos.z + homingDistance;

	//ホーミング方向に座標を移動
	Vector3 toCamera = cameraHomingPos - GetWorldPos();
	toCamera.normalize();
	stageClearMoveVelocity.normalize();
	stageClearModeTimer = min(stageClearModeTimer, (int)maxTime);
	const float speed = 0.5f;
	stageClearMoveVelocity = Easing::LerpVec3(stageClearMoveVelocity, toCamera, time) * speed;
	position += stageClearMoveVelocity;

	//進行方向を向くようにする
	rotation.y = XMConvertToDegrees(std::atan2(stageClearMoveVelocity.x, stageClearMoveVelocity.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 velocityZ = MatrixTransformDirection(stageClearMoveVelocity, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-velocityZ.y, velocityZ.z));

	//回転角度Zをセット
	const float downward = 180;
	float maxRotZ = (360 * 3 + downward);
	//左側移動時の設定
	if (!isStageClearMoveRight) { maxRotZ = -maxRotZ; }
	rotation.z = Easing::OutQuad(stageClearRota.z, maxRotZ, time);

	//カメラより後ろへいったら次のフェーズへ
	if (position.z <= stageClearCameraPos.z) {
		stageClearModePhase = StageClearModePhase::Up;

		//角度を回転した分引く
		rotation.z = downward;

		//タイマーを初期化
		stageClearModeTimer = 0;
	}
}

void Stage01Player::StageClearUp()
{
	//タイマー更新
	const float upTime = 350;
	stageClearModeTimer++;

	//下向きを上向きに
	const Vector3 rotSpeed = { -0.1f, 0, -1 };
	rotation += rotSpeed;
	const float upRotLimit = 40;
	rotation.x = max(rotation.x, -upRotLimit);
	rotation.z = max(rotation.z, 0);

	//自機が傾いている角度に移動させる
	Vector3 velocity;
	velocity.x = stageClearMoveVelocity.x;
	velocity.y = stageClearMoveVelocity.z * (rotation.x / upRotLimit);
	velocity.z = stageClearMoveVelocity.z;
	position += velocity;

	//タイマーが指定した時間になったら次のフェーズへ
	if (stageClearModeTimer >= upTime) {
		stageClearModePhase = StageClearModePhase::Stay;

		//z軸ゆらゆら回転初期化
		isRotZRight = true;
		swayZ = 0.0f;

		//タイマー初期化
		stageClearModeTimer = 0;
	}
}

void Stage01Player::StageClearStay()
{
	//動きがないと寂しいのでゆらゆらさせておく
	const float rotZSpeed = 0.03f;
	const float rotZLimit = 0.8f;
	//右回転
	if (isRotZRight) {
		swayZ += rotZSpeed;
		if (swayZ >= rotZLimit) {
			isRotZRight = false;
		}
	}
	//左回転
	else {
		swayZ -= rotZSpeed;
		if (swayZ <= -rotZLimit) {
			isRotZRight = true;
		}
	}
	rotation.z = swayZ;


	//速度を0に
	const float decelerationTime = 120;
	if (stageClearModeTimer >= decelerationTime) { return; }
	stageClearModeTimer++;
	const float time = stageClearModeTimer / decelerationTime;
	Vector3 velocity = Easing::LerpVec3(stageClearMoveVelocity, { 0, 0, 0 }, time);

	//だんだん減速
	position += velocity;
}

void Stage01Player::StageClearBoost()
{
	//ブースト移動
	//自機が傾いている角度に移動させる
	Vector3 velocity = { 0, 0, 1.25f };
	velocity = MatrixTransformDirection(velocity, matWorld);
	position += velocity;

	//ステージクリア後の行動が完了していたら抜ける
	if (isStageClearModeCompletion) { return; }

	//タイマー更新
	const float stageClearModeCompletionTime = 120;
	stageClearModeTimer++;

	//タイマーが指定した時間に以下なら抜ける
	if (stageClearModeTimer < stageClearModeCompletionTime) { return; }

	//ステージクリア後の行動が完了したことにする
	isStageClearModeCompletion = true;
}
