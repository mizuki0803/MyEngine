#include "EnemyFireBullet.h"
#include "ParticleEmitter.h"

EnemyFireBullet* EnemyFireBullet::Create(const Vector3& position, const Vector3& velocity, const float size)
{
	//敵火炎弾のインスタンスを生成
	EnemyFireBullet* enemyFireBullet = new EnemyFireBullet();
	if (enemyFireBullet == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!enemyFireBullet->Initialize()) {
		delete enemyFireBullet;
		assert(0);
		return nullptr;
	}

	//座標をセット
	enemyFireBullet->position = position;
	//速度をセット
	enemyFireBullet->velocity = velocity;
	//大きさをセット
	enemyFireBullet->scale = { size, size, size };
	//寿命タイマーをセット
	enemyFireBullet->deathTimer = lifeTime;

	return enemyFireBullet;
}

void EnemyFireBullet::Update()
{
	//弾更新
	EnemyBullet::Update();

	//爆発させる間隔の時間
	const int explosionInterval = 2;
	if (deathTimer % explosionInterval == 0) {
		//爆発
		//船首の口元に合わせる
		Vector3 pos = position;
		//ランダムでずらす
		const Vector3 randPos = { 2, 2, 1 };
		pos.x += (float)((rand() % (int)randPos.x) - randPos.x / 2);
		pos.y += (float)((rand() % (int)randPos.y) - randPos.y / 2);
		pos.z += (float)((rand() % (int)randPos.z));

		//火炎球演出用パーティクル生成
		const float size = scale.x * 1.5f;
		ParticleEmitter::GetInstance()->FireBall(GetWorldPos(), size, 1);
	}
}

void EnemyFireBullet::OnCollision()
{
	//死亡させる
	isDead = true;

	//火炎球死亡演出用パーティクル生成
	const float size = scale.x * 3;
	ParticleEmitter::GetInstance()->FireBall(GetWorldPos(), size, 1);
}
