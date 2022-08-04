#include "CannonEnemy.h"
#include "Player.h"

CannonEnemy* CannonEnemy::Create(ObjModel* model, const Vector3& position)
{
	//敵のインスタンスを生成
	CannonEnemy* cannon = new CannonEnemy();
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

void CannonEnemy::Update()
{
	//発射タイマーカウントダウン
	--fireTimer;
	if (fireTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = fireInterval;
	}

	//3Dオブジェクトの更新
	ObjObject3d::Update();

	//画面外手前まで行ったら削除
	FrontOfScreenDelete();
}
