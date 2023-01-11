#include "BossDeadEffect.h"
#include "ParticleEmitter.h"

BossDeadEffect* BossDeadEffect::Create(const Vector3& deadPos, const float explosionSize, const bool isBlackSmoke, const bool isGround)
{
	//ボス死亡後エフェクトのインスタンスを生成
	BossDeadEffect* bossDeadEffect = new BossDeadEffect();
	if (bossDeadEffect == nullptr) {
		return nullptr;
	}

	//ボス死亡座標をセット
	bossDeadEffect->deadPos = deadPos;
	//爆発の大きさをセット
	bossDeadEffect->explosionSize = explosionSize;
	//黒煙を出すかセット
	bossDeadEffect->isBlackSmoke = isBlackSmoke;
	//地面用の爆発をするかをセット
	bossDeadEffect->isGround = isGround;

	return bossDeadEffect;
}

void BossDeadEffect::Update()
{
	//タイマー更新
	timer++;

	//爆発
	Explosion();

	//黒煙を出す状態でなければ抜ける
	if (!isBlackSmoke) { return; }

	//黒煙
	BlackSmoke();
}

void BossDeadEffect::Explosion()
{
	//既に爆発終了フラグが立っていたら抜ける
	if (isExplosionEnd) { return; }

	//毎フレーム出すと多いので間隔を設定
	const int explosionInterval = 15;
	//指定した間隔以外なら抜ける
	if (timer % explosionInterval != 0) { return; }

	//爆発演出用パーティクル生成
	Vector3 explosionPos = deadPos;
	const float distance = 15.0f;
	explosionPos.x += (float)rand() / RAND_MAX * distance - distance / 2.0f;
	if (!isGround) { explosionPos.y += (float)rand() / RAND_MAX * distance - distance / 2.0f; }
	explosionPos.z += (float)rand() / RAND_MAX * distance - distance / 2.0f;
	ParticleEmitter::GetInstance()->BossDeadExplosion(explosionPos, explosionSize, isGround);

	//爆発演出回数更新
	explosionCount++;
	//爆発が指定した回数発生したら終了フラグを立てる
	const int explosionNum = 5;
	if (explosionCount >= explosionNum) {
		isExplosionEnd = true;
	}
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
