#include "UpDownEnemy.h"
#include "Player.h"
#include "ParticleEmitter.h"

void (UpDownEnemy::* UpDownEnemy::actionFuncTable[])() = {
	&UpDownEnemy::UpBrake,
	&UpDownEnemy::DownBrake,
	&UpDownEnemy::Dead,
};

ObjModel* UpDownEnemy::enemyModel = nullptr;
std::array<ObjModel*, 5> UpDownEnemy::breakModels;

UpDownEnemy* UpDownEnemy::Create(const Vector3& position)
{
	//敵のインスタンスを生成
	UpDownEnemy* upDownEnemy = new UpDownEnemy();
	if (upDownEnemy == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(enemyModel);
	upDownEnemy->model = UpDownEnemy::enemyModel;

	// 初期化
	if (!upDownEnemy->Initialize()) {
		delete upDownEnemy;
		assert(0);
		return nullptr;
	}

	//座標をセット
	upDownEnemy->position = position;

	return upDownEnemy;
}

void UpDownEnemy::SetBreakModel(int modelNum, ObjModel* model)
{
	//破壊時のモデルをセット
	assert(model);
	breakModels[modelNum] = model;
}

void UpDownEnemy::Update()
{
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//オブジェクトの更新
	Enemy::Update();

	//画面外手前まで行ったら削除
	FrontOfScreenDelete();
}

void UpDownEnemy::OnCollision()
{
	//全敵共通の衝突処理
	Enemy::OnCollision();

	//行動を死亡用にする
	phase = Phase::Dead;

	//死亡時墜落回転速度を乱数でセット
	const float randRotVelX = -1.0f;
	const float randRotBaseVelX = -0.5f;
	crashRotVel.x = (float)rand() / RAND_MAX * randRotVelX + randRotBaseVelX;
}

void UpDownEnemy::UpBrake()
{
	//上昇を抑える
	Vector3 accel = { 0, -0.01f, 0 };
	velocity += accel;
	position += velocity;


	//指定した速度まで下降状態になったら下降にブレーキをかける
	const float changePhaseVelY = -0.5f;
	if (velocity.y <= changePhaseVelY) {
		phase = Phase::DownBrake;
	}
}

void UpDownEnemy::DownBrake()
{
	//下降を抑える
	Vector3 accel = { 0, 0.01f, 0 };
	velocity += accel;
	position += velocity;


	//指定した速度まで上昇状態になったら上昇にブレーキをかける
	const float changePhaseVelY = 0.5f;
	if (velocity.y >= changePhaseVelY) {
		phase = Phase::UpBrake;
	}
}

void UpDownEnemy::Dead()
{
	//回転をしながら墜落する
	rotation += crashRotVel;

	//墜落するため、速度に加速度を加える
	Vector3 crashAccel = { 0, -0.01f, 0 };
	crashVel += crashAccel;
	//落下する速度の最大値を設定
	const float maxCrashSpeed = -0.5f;
	if (crashVel.y <= maxCrashSpeed) { crashVel.y = maxCrashSpeed; }
	position += crashVel;

	//Y座標が0以下になったら削除
	if (position.y <= 0) {
		isDelete = true;

		//爆発演出用パーティクル生成
		ParticleEmitter::GetInstance()->Explosion(position);
	}
}

void UpDownEnemy::Break()
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
