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
