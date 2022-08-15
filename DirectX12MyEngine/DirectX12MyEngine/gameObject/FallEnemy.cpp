#include "FallEnemy.h"
#include "Easing.h"

void (FallEnemy::* FallEnemy::actionFuncTable[])() = {
	&FallEnemy::Fall,
	&FallEnemy::Attack,
};

FallEnemy* FallEnemy::Create(ObjModel* model, const Vector3& position)
{
	//敵のインスタンスを生成
	FallEnemy* upDownEnemy = new FallEnemy();
	if (upDownEnemy == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	upDownEnemy->model = model;

	// 初期化
	if (!upDownEnemy->Initialize()) {
		delete upDownEnemy;
		assert(0);
		return nullptr;
	}

	//初期座標をセット
	upDownEnemy->startPos = position;

	//座標をセット
	upDownEnemy->position = position;

	return upDownEnemy;
}

void FallEnemy::Update()
{
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//3Dオブジェクトの更新
	ObjObject3d::Update();

	//画面外手前まで行ったら削除
	FrontOfScreenDelete();	
}

void FallEnemy::Fall()
{
	//イージングで降下する
	const float fallTime = 60;
	fallTimer++;
	const float time = fallTimer / fallTime;

	//生成位置から100降りたところで停止する
	const float stayPosY = startPos.y - 100;
	position.y = Easing::OutQuart(startPos.y, stayPosY, time);

	//イージングが終了したら次のフェーズへ
	if (fallTimer >= fallTime) {
		phase = Phase::Attack;
	}
}

void FallEnemy::Attack()
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
