#include "Cannon.h"
#include "Player.h"

Cannon* Cannon::Create(ObjModel* model, const Vector3& position)
{
	//敵のインスタンスを生成
	Cannon* cannon = new Cannon();
	if (cannon == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	cannon->model = model;

	// 初期化
	if (!cannon->Initialize()) {
		delete cannon;
		assert(0);
		return nullptr;
	}


	//座標をセット
	cannon->position = position;

	return cannon;
}

void Cannon::Update()
{
	//発射タイマーカウントダウン
	--fireTimer;
	if (fireTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = fireInterval;
	}

	//画面外手前まで行ったら削除
	FrontOfScreenDelete();

	//3Dオブジェクトの更新
	ObjObject3d::Update();
}

void Cannon::FrontOfScreenDelete()
{
	//座標が自機より手前(画面外手前)まで行ったら削除
	const float flontOfScreenDiffence = 100;
	if (GetWorldPos().z <= player->GetWorldPos().z - flontOfScreenDiffence) {
		isDead = true;
	}
}
