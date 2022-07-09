#include "PlayerBullet.h"

PlayerBullet* PlayerBullet::Create(ObjModel* model, XMFLOAT3 position)
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
	//3Dオブジェクトの更新
	ObjObject3d::Update();
}