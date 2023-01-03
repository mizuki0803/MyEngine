#include "Stage02Player.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (Stage02Player::* Stage02Player::stageClearActionFuncTable[])() = {
	&Stage02Player::StageClearStay,
	&Stage02Player::StageClearAdvance,
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
	//基準の座標をセット
	basePos = { 0, -2, 16 };

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

	//パーティクルの大きさを統一するため、移動はもうしないが通常移動状態にしておく
	moveSpeedPhase = MoveSpeedPhase::NormalSpeed;

	//ステージクリア後の動きをする
	isStageClearMode = true;
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
	const Vector3 rotSpeed = { 0.25f, 0, 4 + crashTimer * 0.01f };
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
	const int crashTimeMax = 240;
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

void Stage02Player::StageClearStay()
{
	//角度を0していく
	StageClearRotateFix();

	//自機が傾いている角度に移動させる
	Vector3 velocity = { 0, 0, 0 };
	velocity.x = moveBaseSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveBaseSpeed * -(rotation.x / rotLimit.x);
	position += velocity;

	//ブラーの強さを通常移動時の強さに戻していく
	SpeedChangeNormalBlur();
}

void Stage02Player::StageClearRotateFix()
{
	//回転速度
	const float rotSpeed = 1.0f;
	//角度修正基準速度
	const float backBaseSpeed = rotSpeed / 1.8f;
	Vector3 rot = { 0, 0, 0 };

	//y軸回転
	{
		//角度修正速度倍率
		float backSpeedRatio = fabsf(rotation.y / (rotLimit.y * 2)) + 0.5f;
		//角度修正速度
		const float backSpeed = backBaseSpeed * backSpeedRatio;
		//y軸回転の傾きを修正する
		const float rotMin = 0.5f;
		if (rotation.y > rotMin) {
			rot.y -= backSpeed;
		}
		else if (rotation.y < -rotMin) {
			rot.y += backSpeed;
		}
		else {
			rotation.y = 0;
		}
	}
	//x軸回転
	{
		//角度修正速度倍率
		float backSpeedRatio = fabsf(rotation.x / (rotLimit.x * 2)) + 0.5f;
		//角度修正速度
		const float backSpeed = backBaseSpeed * backSpeedRatio;
		//x軸回転の傾きを修正する
		const float rotMin = 0.5f;
		if (rotation.x > rotMin) {
			rot.x -= backSpeed;
		}
		else if (rotation.x < -rotMin) {
			rot.x += backSpeed;
		}
		else {
			rotation.x = 0;
		}
	}
	//z軸回転
	{
		//動きがないと寂しいのでゆらゆらさせておく
		const float rotZSpeed = 0.04f;
		const float rotZLimit = 1.0f;
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

		rotation.z = -rotation.y + swayZ;
	}

	//回転の更新
	rotation += rot;

	//角度の限界値からはみ出さない
	rotation.y = max(rotation.y, -rotLimit.y);
	rotation.y = min(rotation.y, +rotLimit.y);
	rotation.x = max(rotation.x, -rotLimit.x);
	rotation.x = min(rotation.x, +rotLimit.x);
}

void Stage02Player::StageClearAdvance()
{
	//自機が傾いている角度に前進させる
	Vector3 velocity = { 0, 0, 0.2f };
	velocity = MatrixTransformDirection(velocity, matWorld);
	position += velocity;
}

void Stage02Player::StageClearBoost()
{
	//ブースト移動
	//自機が傾いている角度に移動させる
	Vector3 velocity = { 0, 0, 1.5f };
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
