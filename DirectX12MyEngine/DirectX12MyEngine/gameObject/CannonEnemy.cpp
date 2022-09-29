#include "CannonEnemy.h"
#include "Player.h"

void (CannonEnemy::* CannonEnemy::actionFuncTable[])() = {
	&CannonEnemy::Attack,
	&CannonEnemy::Dead,
};

ObjModel* CannonEnemy::breakModel = nullptr;

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
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//3Dオブジェクトの更新
	ObjObject3d::Update();

	//画面外手前まで行ったら削除
	FrontOfScreenDelete();
}

void CannonEnemy::OnCollision()
{
	//死亡しても削除しないモデル変更のため、死亡していたら抜ける
	if (isDead) { return; }

	//全敵共通の衝突処理
	Enemy::OnCollision();

	//行動を死亡用にする
	phase = Phase::Dead;

	//破壊されたモデルに変更
	model = breakModel;
}

void CannonEnemy::Attack()
{
	//発射タイマーカウントダウン
	--fireTimer;
	if (fireTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = fireInterval;
	}
}

void CannonEnemy::Dead()
{
}
