#include "ComeGoEnemy.h"
#include "Easing.h"

void (ComeGoEnemy::*ComeGoEnemy::actionFuncTable[])() = {
	&ComeGoEnemy::Come,
	&ComeGoEnemy::Attack,
	&ComeGoEnemy::Go
};

ComeGoEnemy* ComeGoEnemy::Create(ObjModel* model, const Vector3& startPos, const Vector3& comePos, const Vector3& goTargetPos, const int attackTime)
{
	//敵のインスタンスを生成
	ComeGoEnemy* comeGoEnemy = new ComeGoEnemy();
	if (comeGoEnemy == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	comeGoEnemy->model = model;

	// 初期化
	if (!comeGoEnemy->Initialize()) {
		delete comeGoEnemy;
		assert(0);
		return nullptr;
	}

	//生成座標をセット
	comeGoEnemy->startPos = startPos;

	//到着座標をセット
	comeGoEnemy->comePos = comePos;

	//出発目標座標をセット
	comeGoEnemy->goTargetPos = goTargetPos;

	//攻撃時間をセット
	comeGoEnemy->attackTime = attackTime;

	return comeGoEnemy;
}

void ComeGoEnemy::Update()
{
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//3Dオブジェクトの更新
	ObjObject3d::Update();
}

void ComeGoEnemy::Come()
{
	//イージングで到着させる
	const float comeTime = 120;
	comeTimer++;
	const float time = comeTimer / comeTime;
	position.x = Easing::OutQuart(startPos.x, comePos.x, time);
	position.y = Easing::OutQuart(startPos.y, comePos.y, time);
	position.z = Easing::OutQuart(startPos.z, comePos.z, time);

	//イージングが終了したら次のフェーズへ
	if (comeTimer >= comeTime) {
		phase = Phase::Attack;
	}
}

void ComeGoEnemy::Attack()
{
	//攻撃する時間をカウント
	attackTimer++;

	//Z方向に移動させる
	Vector3 velocity = { 0, 0, 0.1f };
	position += velocity;

	//発射タイマーカウントダウン
	--fireTimer;
	if (fireTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = fireInterval;
	}

	//時間になったら次のフェーズへ
	if (attackTimer >= attackTime) {
		phase = Phase::Go;

		//出発開始座標をセット
		goStartPos = position;
	}
}

void ComeGoEnemy::Go()
{
	//イージングで出発させる
	const float goTime = 120;
	goTimer++;
	const float time = goTimer / goTime;
	position.x = Easing::InQuart(goStartPos.x, goTargetPos.x, time);
	position.y = Easing::InQuart(goStartPos.y, goTargetPos.y, time);
	position.z = Easing::InQuart(goStartPos.z, goTargetPos.z, time);

	//イージングが終了したら削除する
	if (goTimer >= goTime) {
		isDead = true;
	}
}
