#include "MeteoriteEnemy.h"
#include "ParticleEmitter.h"

ObjModel* MeteoriteEnemy::meteoriteModel = nullptr;

MeteoriteEnemy* MeteoriteEnemy::Create(const Vector3& position, const Vector3& rotation, float size, const Vector3& velocity, const Vector3& rotSpeed, int HP)
{
	//隕石のインスタンスを生成
	MeteoriteEnemy* meteoriteEnemy = new MeteoriteEnemy();
	if (meteoriteEnemy == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!meteoriteEnemy->Initialize()) {
		delete meteoriteEnemy;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(meteoriteModel);
	meteoriteEnemy->model = meteoriteModel;
	//座標をセット
	meteoriteEnemy->position = position;
	//回転角をセット
	meteoriteEnemy->rotation = rotation;
	//大きさをセット
	meteoriteEnemy->scale = { size, size, size };
	//通常サイズをセット
	meteoriteEnemy->normalSize = meteoriteEnemy->scale;
	//ダメージ状態のサイズをセット
	meteoriteEnemy->damageSize = meteoriteEnemy->scale * 1.1f;
	//移動速度をセット
	meteoriteEnemy->velocity = velocity;
	//回転速度をセット
	meteoriteEnemy->rotSpeed = rotSpeed;
	//HPをセット
	meteoriteEnemy->HP = HP;

	return meteoriteEnemy;
}

void MeteoriteEnemy::Update()
{
	//座標を更新
	position += velocity;
	//回転角を更新
	rotation += rotSpeed;

	//死亡状態
	if (isDead) {
		Dead();
	}

	//オブジェクト更新
	Enemy::Update();

	//手前まで行ったら削除
	FrontOfScreenDelete();
}

void MeteoriteEnemy::Dead()
{
	//削除までにかかる時間
	const int deadModeTime = 2;
	//タイマーを更新
	deadTimer++;

	//タイマーが指定した時間になったら
	if (deadTimer >= deadModeTime) {
		//削除する
		isDelete = true;

		//破壊エフェクトを出す
		Break();

		//爆発用大きさ
		float explosionSize = 4;
		//爆発時間
		int explosionTime = 30;
		//爆発演出用パーティクル生成
		ParticleEmitter::GetInstance()->Explosion(position, explosionSize, explosionTime);
	}
}

void MeteoriteEnemy::Break()
{
	//破壊用エフェクト追加
	for (int i = 0; i < 10; i++) {
		//モデルが未設定なら飛ばす
		if (!meteoriteModel) { continue; }

		//ランダムでエフェクトの速度をセット
		const Vector3 randVel = { 6, 6, 6 };
		Vector3 velocity;
		velocity.x = (float)((rand() % (int)randVel.x) - randVel.x / 2);
		velocity.y = (float)((rand() % (int)randVel.y) - randVel.y / 2);
		velocity.z = (float)((rand() % (int)randVel.z) - randVel.z / 2);

		//ランダムでエフェクトの回転の速さをセット
		const Vector3 randRotSpeed = { 5, 5, 5 };
		Vector3 rotSpeed;
		rotSpeed.x = (float)((rand() % (int)randRotSpeed.x) - randRotSpeed.x / 2);
		rotSpeed.y = (float)((rand() % (int)randRotSpeed.y) - randRotSpeed.y / 2);
		rotSpeed.z = (float)((rand() % (int)randRotSpeed.z) - randRotSpeed.z / 2);

		//値が大きいので割り算して小さくする
		const float div = 4;
		velocity /= div;
		//大きさをセット
		const Vector3 scale = this->scale / 5;

		//破壊時エフェクト生成
		BreakEffect(meteoriteModel, velocity, rotSpeed, scale);
	}
}
