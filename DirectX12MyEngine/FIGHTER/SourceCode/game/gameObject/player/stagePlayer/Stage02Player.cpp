#include "Stage02Player.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (Stage02Player::* Stage02Player::stageClearActionFuncTable[])() = {
	&Stage02Player::StageClearSideMove,
	&Stage02Player::StageClearReturn,
	&Stage02Player::StageClearUp,
	&Stage02Player::StageClearStay,
	&Stage02Player::StageClearBoost,
};

Stage02Player* Stage02Player::Create(ObjModel* model, const int startHP, const int maxHP)
{
	//ステージ02自機のインスタンスを生成
	Stage02Player* player = new Stage02Player();
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

bool Stage02Player::Initialize(ObjModel* model, const int startHP, const int maxHP)
{
	//自機の共通初期化
	if (!BasePlayer::Initialize(model, startHP, maxHP)) {
		return false;
	}

	//自機の移動限界をセット
	moveLimitMin = { -15.0f, -8.0f };
	moveLimitMax = { 15.0f, 8.0f };

	return true;
}

void Stage02Player::StageClearModeStart()
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

void Stage02Player::StageClearReturnStart(const Vector3& cameraPos)
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

void Stage02Player::StageClearBoostStart()
{
	//ブースト状態にする
	stageClearModePhase = StageClearModePhase::Boost;
}

void Stage02Player::CrashStart()
{
	//少し下向きに修正しておく
	const float crashStartRota = 10;
	rotation.x = crashStartRota;

	//全敵共通の墜落開始
	BasePlayer::CrashStart();
}

void Stage02Player::Crash()
{
	//墜落回転する
	const Vector3 rotSpeed = { 0.25f, 0, 5 + crashTimer * 0.01f };
	rotation += rotSpeed;
	const float crashRotLimit = 90;
	rotation.x = min(rotation.x, crashRotLimit);

	//自機が傾いている角度に移動させる
	const float crashMoveSpeed = 0.5f;
	Vector3 velocity = { moveBaseSpeed, 1.0f + crashTimer * 0.01f, crashMoveSpeed };
	velocity.x *= (rotation.y / rotLimit.y);
	velocity.y *= -(rotation.x / crashRotLimit);
	velocity.z *= (1 - (rotation.x / crashRotLimit));
	//前に進む速度は0にならないようにしておくため、最小値を設定
	const Vector3 velLimit = { 0, -1.5f, 0.1f };
	velocity.y = max(velocity.y, velLimit.y);
	velocity.z = max(velocity.z, velLimit.z);
	position += velocity;

	//ブラーの強さを通常移動時の強さに戻していく
	SpeedChangeNormalBlur();

	//墜落タイマー更新
	crashTimer++;
	//墜落状態の最大時間を越えていなければ抜ける
	const int crashTimeMax = 300;
	if (crashTimer < crashTimeMax) { return; }

	//死亡
	isDead = true;

	//爆発演出用パーティクル生成
	const float explosionSize = 2.5f;
	const int explosionTime = 60;
	ParticleEmitter::GetInstance()->Explosion(position, explosionSize, explosionTime);
}

void Stage02Player::StageClear()
{
	//ステージクリア後行動
	(this->*stageClearActionFuncTable[static_cast<size_t>(stageClearModePhase)])();
}

void Stage02Player::StageClearSideMove()
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

void Stage02Player::StageClearReturn()
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

void Stage02Player::StageClearUp()
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

void Stage02Player::StageClearStay()
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

void Stage02Player::StageClearBoost()
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
