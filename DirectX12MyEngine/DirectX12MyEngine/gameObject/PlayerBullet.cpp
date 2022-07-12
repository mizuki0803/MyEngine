#include "PlayerBullet.h"

PlayerBullet* PlayerBullet::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
{
	//自機弾のインスタンスを生成
	PlayerBullet* playerBullet = new PlayerBullet();
	if (playerBullet == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!playerBullet->Initialize()) {
		delete playerBullet;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(model);
	playerBullet->model = model;

	//座標をセット
	playerBullet->position = position;

	//速度をセット
	playerBullet->velocity = velocity;

	return playerBullet;
}

bool PlayerBullet::Initialize()
{
	scale = { 0.1f, 0.1f, 0.1f };

	//3Dオブジェクトの初期化
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	return true;
}

void PlayerBullet::Update()
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
