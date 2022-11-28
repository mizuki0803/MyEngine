#include "ComeGoEnemy.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (ComeGoEnemy::* ComeGoEnemy::actionFuncTable[])() = {
	&ComeGoEnemy::Come,
	&ComeGoEnemy::Attack,
	&ComeGoEnemy::Go,
	&ComeGoEnemy::Dead,
};

float ComeGoEnemy::attackMoveSpeed = 0;
ObjModel* ComeGoEnemy::enemyModel = nullptr;
std::array<ObjModel*, 5> ComeGoEnemy::breakModels;

ComeGoEnemy* ComeGoEnemy::Create(const Vector3& startPos, const Vector3& comePos, const Vector3& goTargetPos, const int attackTime)
{
	//敵のインスタンスを生成
	ComeGoEnemy* comeGoEnemy = new ComeGoEnemy();
	if (comeGoEnemy == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(enemyModel);
	comeGoEnemy->model = ComeGoEnemy::enemyModel;

	// 初期化
	if (!comeGoEnemy->Initialize()) {
		delete comeGoEnemy;
		assert(0);
		return nullptr;
	}

	//生成座標をセット
	comeGoEnemy->startPos = startPos;

	//到着座標をセット
	comeGoEnemy->comePos = comePos;

	//出発目標座標をセット
	comeGoEnemy->goTargetPos = goTargetPos;

	//攻撃時間をセット
	comeGoEnemy->attackTime = attackTime;

	return comeGoEnemy;
}

void ComeGoEnemy::SetBreakModel(int modelNum, ObjModel* model)
{
	//破壊時のモデルをセット
	assert(model);
	breakModels[modelNum] = model;
}

void ComeGoEnemy::Update()
{
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//オブジェクトの更新
	Enemy::Update();
}

void ComeGoEnemy::OnCollision()
{
	//全敵共通の衝突処理
	Enemy::OnCollision();

	//行動を死亡用にする
	phase = Phase::Dead;

	//死亡時墜落回転速度を乱数でセット
	const Vector3 randRotVel = { 0, 0.4f, 4.0f };
	crashRotVel.x = (float)rand() / RAND_MAX * randRotVel.x - randRotVel.y / 2;
	crashRotVel.y = (float)rand() / RAND_MAX * randRotVel.y - randRotVel.y / 2;
	crashRotVel.z = (float)rand() / RAND_MAX * randRotVel.z - randRotVel.z / 2;
}

void ComeGoEnemy::Come()
{
	//イージングで到着させる
	const float comeTime = 120;
	comeTimer++;
	const float time = comeTimer / comeTime;
	position.x = Easing::OutQuart(startPos.x, comePos.x, time);
	position.y = Easing::OutQuart(startPos.y, comePos.y, time);
	position.z = Easing::OutQuart(startPos.z, comePos.z, time);

	//イージングが終了したら次のフェーズへ
	if (comeTimer >= comeTime) {
		phase = Phase::Attack;
	}
}

void ComeGoEnemy::Attack()
{
	//攻撃する時間をカウント
	attackTimer++;

	//Z方向に移動させる
	position.z += attackMoveSpeed;

	//発射タイマーカウントダウン
	--fireTimer;
	if (fireTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = fireInterval;
	}

	//時間になったら次のフェーズへ
	if (attackTimer >= attackTime) {
		phase = Phase::Go;

		//出発開始座標をセット
		goStartPos = position;
	}
}

void ComeGoEnemy::Go()
{
	//イージングで出発させる
	const float goTime = 120;
	goTimer++;
	const float time = goTimer / goTime;
	position.x = Easing::InQuart(goStartPos.x, goTargetPos.x, time);
	position.y = Easing::InQuart(goStartPos.y, goTargetPos.y, time);
	position.z = Easing::InQuart(goStartPos.z, goTargetPos.z, time);

	//イージングが終了したら削除する
	if (goTimer >= goTime) {
		isDelete = true;
	}
}

void ComeGoEnemy::Dead()
{
	//回転をしながら墜落する
	rotation += crashRotVel;

	//墜落するため、速度に加速度を加える
	Vector3 crashAccel = { 0, -0.005f, 0 };
	crashVel += crashAccel;
	//落下する速度の最大値を設定
	const float maxCrashSpeed = -0.2f;
	if (crashVel.y <= maxCrashSpeed) { crashVel.y = maxCrashSpeed; }
	position += crashVel;

	//Y座標が0以下になったら削除
	if (position.y <= 0) {
		isDelete = true;

		//爆発演出用パーティクル生成
		ParticleEmitter::GetInstance()->Explosion(position);
	}
}

void ComeGoEnemy::Break()
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
		const Vector3 randRotSpeed = { 10, 10, 10 };
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
