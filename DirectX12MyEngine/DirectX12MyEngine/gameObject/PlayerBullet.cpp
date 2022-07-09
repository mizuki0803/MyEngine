#include "PlayerBullet.h"

PlayerBullet* PlayerBullet::Create(ObjModel* model, const XMFLOAT3& position, const XMFLOAT3& velocity)
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
	playerBullet->SetModel(model);

	//座標をセット
	playerBullet->SetPosition(position);

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
	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;

	//時間経過で死亡
	if (--deathTimer <= 0) {
		isDead = true;
	}

	//3Dオブジェクトの更新
	ObjObject3d::Update();
}
