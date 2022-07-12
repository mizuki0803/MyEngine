#include "EnemyBullet.h"

EnemyBullet* EnemyBullet::Create(ObjModel* model, Vector3& position, Vector3& velocity)
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

	return enemyBullet;
}

bool EnemyBullet::Initialize()
{
	scale = { 0.1f, 0.1f, 0.1f };

	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	return true;
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
