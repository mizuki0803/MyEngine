#include "EnemyBullet.h"

EnemyBullet* EnemyBullet::Create(ObjModel* model, const Vector3& position, const Vector3& velocity, const float scale)
{
	//弾弾のインスタンスを生成
	EnemyBullet* enemyBullet = new EnemyBullet();
	if (enemyBullet == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!enemyBullet->Initialize()) {
		delete enemyBullet;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(model);
	enemyBullet->model = model;

	//座標をセット
	enemyBullet->position = position;

	//速度をセット
	enemyBullet->velocity = velocity;

	//大きさをセット
	enemyBullet->scale = { scale, scale, scale };

	return enemyBullet;
}

void EnemyBullet::Update()
{
	//座標を移動
	position += velocity;

	//時間経過で死亡
	if (--deathTimer <= 0) {
		isDead = true;
	}

	//3Dオブジェクトの更新
	ObjObject3d::Update();
}

void EnemyBullet::OnCollision()
{
	//死亡させる
	isDead = true;
}

Vector3 EnemyBullet::GetWorldPos()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//平行移動成分を取得
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}
