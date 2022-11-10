#include "PlayerBullet.h"

bool PlayerBullet::Initialize()
{
	scale = { 0.6f, 0.6f, 0.6f };

	//3Dオブジェクトの初期化
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	return true;
}

void PlayerBullet::Update()
{
	//地面に触れたとき
	CollisionGround();

	//オブジェクト更新
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

void PlayerBullet::CollisionGround()
{
	//Y座標0以下でなければ抜ける
	if (position.y > 0) { return; }

	//死亡
	isDead = true;
}
