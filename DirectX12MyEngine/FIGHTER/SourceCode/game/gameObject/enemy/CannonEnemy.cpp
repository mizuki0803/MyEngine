#include "CannonEnemy.h"
#include "ParticleEmitter.h"

void (CannonEnemy::* CannonEnemy::actionFuncTable[])() = {
	&CannonEnemy::Attack,
	&CannonEnemy::Dead,
};

ObjModel* CannonEnemy::enemyModel = nullptr;
std::array<ObjModel*, 5> CannonEnemy::breakModels;

CannonEnemy* CannonEnemy::Create(const Vector3& position)
{
	//敵のインスタンスを生成
	CannonEnemy* cannon = new CannonEnemy();
	if (cannon == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(enemyModel);
	cannon->model = CannonEnemy::enemyModel;

	// 初期化
	if (!cannon->Initialize()) {
		delete cannon;
		assert(0);
		return nullptr;
	}

	//座標をセット
	cannon->position = position;

	//大きさをセット
	const float scaleNum = 3;
	cannon->scale = { scaleNum, scaleNum, scaleNum };
	//通常サイズをセット
	cannon->normalSize = cannon->scale;
	//ダメージ状態のサイズをセット
	cannon->damageSize = cannon->scale * 1.1f;

	return cannon;
}

void CannonEnemy::SetBreakModel(int modelNum, ObjModel* model)
{
	//破壊時のモデルをセット
	assert(model);
	breakModels[modelNum] = model;
}

void CannonEnemy::Update()
{
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//オブジェクトの更新
	Enemy::Update();

	//画面外手前まで行ったら削除
	FrontOfScreenDelete();
}

void CannonEnemy::OnCollision()
{
	//全敵共通の衝突処理
	Enemy::OnCollision();

	//行動を死亡用にする
	phase = Phase::Dead;

	//死亡時墜落回転速度を乱数でセット
	const Vector3 randRotVel = { -2.0f, 0.4f, 0.4f };
	const float randRotBaseVelX = -1.75f;
	crashRotVel.x = (float)rand() / RAND_MAX * randRotVel.x + randRotBaseVelX;
	crashRotVel.y = (float)rand() / RAND_MAX * randRotVel.y - randRotVel.y / 2;
	crashRotVel.z = (float)rand() / RAND_MAX * randRotVel.z - randRotVel.z / 2;
}

void CannonEnemy::Attack()
{
	//発射タイマーカウントダウン
	--fireTimer;
	if (fireTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = fireInterval;
	}
}

void CannonEnemy::Dead()
{
	//回転をしながら墜落する
	rotation += crashRotVel;

	//墜落するため、速度に加速度を加える
	Vector3 crashAccel = { 0, -0.005f, 0.001f };
	crashVel += crashAccel;
	//落下する速度の最大値を設定
	const float maxCrashSpeed = -0.2f;
	if (crashVel.y <= maxCrashSpeed) { crashVel.y = maxCrashSpeed; }
	position += crashVel;

	//黒煙パーティクル生成
	const float smokeSize = scale.x * 2.0f;
	DeadSmokeEffect(smokeSize);

	//地面ありのとき
	if (isGroundMode) {
		//Y座標が0以下になったら削除
		if (position.y <= 0) {
			isDelete = true;

			//爆発演出用パーティクル生成
			ParticleEmitter::GetInstance()->Explosion(position);
		}
	}
	//地面なしのとき
	else {
		//Y座標が指定した値以下になったら削除
		const float deletePos = -50;
		if (position.y <= deletePos) {
			isDelete = true;
		}
	}
}

void CannonEnemy::Break()
{
	//破壊用エフェクト追加
	for (int i = 0; i < breakModels.size(); i++) {
		//モデルが未設定なら飛ばす
		if (!breakModels[i]) { continue; }

		//ランダムでエフェクトの速度をセット
		const Vector3 randVel = { 4, 4, 4 };
		Vector3 velocity;
		velocity.x = (float)((rand() % (int)randVel.x) - randVel.x / 2);
		velocity.y = (float)((rand() % (int)randVel.y) - randVel.y / 4);
		velocity.z = (float)((rand() % (int)randVel.z) - randVel.z / 2);

		//ランダムでエフェクトの回転の速さをセット
		const Vector3 randRotSpeed = { 5, 5, 5 };
		Vector3 rotSpeed;
		rotSpeed.x = (float)((rand() % (int)randRotSpeed.x) - randRotSpeed.x / 2);
		rotSpeed.x = (float)((rand() % (int)randRotSpeed.y) - randRotSpeed.y / 2);
		rotSpeed.z = (float)((rand() % (int)randRotSpeed.z) - randRotSpeed.z / 2);

		//値が大きいので割り算して小さくする
		const float div = 10;
		velocity /= div;
		//大きさをセット
		const Vector3 scale = { 1.5f, 1.5f, 1.5f };

		//破壊時エフェクト生成
		BreakEffect(breakModels[i], velocity, rotSpeed, scale);
	}
}
