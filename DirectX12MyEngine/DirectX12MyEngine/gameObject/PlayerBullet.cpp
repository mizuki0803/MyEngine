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
	scale = { 0.3f, 0.3f, 0.3f };

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

void PlayerBullet::OnCollision()
{
	//死亡させる
	isDead = true;
}

Vector3 PlayerBullet::GetWorldPos()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//平行移動成分を取得
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}
