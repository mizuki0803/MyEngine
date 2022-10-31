#include "FallEnemy.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (FallEnemy::*FallEnemy::actionFuncTable[])() = {
	&FallEnemy::Fall,
	&FallEnemy::Attack,
	&FallEnemy::Dead,
};

FallEnemy* FallEnemy::Create(ObjModel* model, const Vector3& position)
{
	//敵のインスタンスを生成
	FallEnemy* upDownEnemy = new FallEnemy();
	if (upDownEnemy == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	upDownEnemy->model = model;

	// 初期化
	if (!upDownEnemy->Initialize()) {
		delete upDownEnemy;
		assert(0);
		return nullptr;
	}

	//初期座標をセット
	upDownEnemy->startPos = position;

	//座標をセット
	upDownEnemy->position = position;

	return upDownEnemy;
}

void FallEnemy::Update()
{
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//オブジェクトの更新
	Enemy::Update();

	//画面外手前まで行ったら削除
	FrontOfScreenDelete();	
}

void FallEnemy::OnCollision()
{
	//全敵共通の衝突処理
	Enemy::OnCollision();

	//行動を死亡用にする
	phase = Phase::Dead;

	//死亡時墜落回転速度を乱数でセット
	const Vector3 randRotVel = { -1.0f, 0.4f, 0.4f };
	const float randRotBaseVelX = -0.25f;
	crashRotVel.x = (float)rand() / RAND_MAX * randRotVel.x + randRotBaseVelX;
	crashRotVel.y = (float)rand() / RAND_MAX * randRotVel.y - randRotVel.y / 2;
	crashRotVel.z = (float)rand() / RAND_MAX * randRotVel.z - randRotVel.z / 2;
}

void FallEnemy::Fall()
{
	//イージングで降下する
	const float fallTime = 60;
	fallTimer++;
	const float time = fallTimer / fallTime;

	//生成位置から100降りたところで停止する
	const float stayPosY = startPos.y - 100;
	position.y = Easing::OutQuart(startPos.y, stayPosY, time);

	//イージングが終了したら次のフェーズへ
	if (fallTimer >= fallTime) {
		phase = Phase::Attack;
	}
}

void FallEnemy::Attack()
{
	//発射タイマーカウントダウン
	--fireTimer;
	if (fireTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = fireInterval;
	}

	//クルクル回転
	const float rotSpeed = 1.0f;
	rotation.y += rotSpeed;
}

void FallEnemy::Dead()
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
		ParticleEmitter::GetInstance()->Explosion(GetWorldPos());
	}
}
