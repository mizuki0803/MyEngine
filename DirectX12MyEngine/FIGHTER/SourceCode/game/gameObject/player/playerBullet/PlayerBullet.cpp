#include "PlayerBullet.h"

bool PlayerBullet::isGroundMode = true;

void PlayerBullet::Update()
{
	//地面あり行動を行う場合のみ
	if (isGroundMode) {
		//地面に触れたときの挙動
		CollisionGround();
	}

	//オブジェクト更新
	ObjObject3d::Update();
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
