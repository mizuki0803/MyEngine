#include "BossChargeBullet.h"
#include "ParticleEmitter.h"

BossChargeBullet* BossChargeBullet::Create(const Vector3& position, const Vector3& velocity, const float size)
{
	//ボスチャージ弾のインスタンスを生成
	BossChargeBullet* bossChargeBullet = new BossChargeBullet();
	if (bossChargeBullet == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!bossChargeBullet->Initialize()) {
		delete bossChargeBullet;
		assert(0);
		return nullptr;
	}

	//座標をセット
	bossChargeBullet->position = position;

	//速度をセット
	bossChargeBullet->velocity = velocity;

	//大きさをセット
	bossChargeBullet->scale = { size, size, size };

	return bossChargeBullet;
}

void BossChargeBullet::Update()
{
	//弾更新
	EnemyBullet::Update();

	//ボスチャージショット演出用パーティクル生成
	ParticleEmitter::GetInstance()->BossChargeShot(GetWorldPos(), scale.x);
}

void BossChargeBullet::OnCollision()
{
	//死亡させる
	isDead = true;

	//ボスチャージショット死亡演出用パーティクル生成
	const float blastSize = scale.x * 3;
	ParticleEmitter::GetInstance()->BossChargeShotDead(GetWorldPos(), blastSize);
}
