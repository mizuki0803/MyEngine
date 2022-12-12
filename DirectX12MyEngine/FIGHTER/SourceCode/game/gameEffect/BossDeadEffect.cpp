#include "BossDeadEffect.h"
#include "ParticleEmitter.h"

BossDeadEffect* BossDeadEffect::Create(const Vector3& deadPos)
{
	//弾弾のインスタンスを生成
	BossDeadEffect* bossDeadEffect = new BossDeadEffect();
	if (bossDeadEffect == nullptr) {
		return nullptr;
	}

	//ボス死亡座標をセット
	bossDeadEffect->deadPos = deadPos;

	return bossDeadEffect;
}

void BossDeadEffect::Update()
{
	//タイマー更新
	timer++;

	//爆発
	Explosion();
	//黒煙
	BlackSmoke();
}

void BossDeadEffect::Explosion()
{
	//既に爆発回数が十分なら抜ける
	const int explosionNum = 7;
	if (explosionCount >= explosionNum) { return; }

	//毎フレーム出すと多いので間隔を設定
	const int explosionInterval = 12;
	//指定した間隔以外なら抜ける
	if (timer % explosionInterval != 0) { return; }

	//爆発演出用パーティクル生成
	Vector3 explosionPos = deadPos;
	const float distance = 15.0f;
	explosionPos.x += (float)rand() / RAND_MAX * distance - distance / 2.0f;
	explosionPos.z += (float)rand() / RAND_MAX * distance - distance / 2.0f;
	ParticleEmitter::GetInstance()->BossDeadExplosion(explosionPos);

	//爆発演出回数更新
	explosionCount++;
}

void BossDeadEffect::BlackSmoke()
{
	//死亡してから黒煙を出すまでの時間
	const int smokeStartTime = 40;
	//タイマーが黒煙を出すまでの時間以下なら抜ける
	if (timer < smokeStartTime) { return; }

	//毎フレーム出すと多いので間隔を設定
	const int smokeInterval = 5;
	//指定した間隔以外なら抜ける
	if (timer % smokeInterval != 0) { return; }

	//死亡座標に黒煙パーティクル生成
	const float smokeSize = 10.0f;
	const int smokeAliveTime = 200;
	ParticleEmitter::GetInstance()->BlackSmoke(deadPos, smokeSize, smokeAliveTime);
}
