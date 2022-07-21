#include "Cannon.h"

Cannon* Cannon::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
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

	//速度をセット
	cannon->velocity = velocity;

	return cannon;
}

bool Cannon::Initialize()
{
	//3Dオブジェクトの初期化
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	PreviousPhaseInit();

	return true;
}

void Cannon::Update()
{
	//移動
	Move();

	//3Dオブジェクトの更新
	ObjObject3d::Update();
}

//void Cannon::OnCollision()
//{
//}

void Cannon::PreviousPhaseInit()
{
	//発射タイマーを初期化
	fireTimer = fireInterval;
}

void Cannon::Move()
{
	switch (phase)
	{
	case Phase::Previous:
	default:
		//前進する
		position -= velocity;

		//一定の位置まで前進したら後退
		if (position.z < 0.0f) {
			phase = Phase::Back;
		}

		//発射タイマーカウントダウン
		--fireTimer;
		if (fireTimer <= 0) {
			//弾を発射
			Fire();
			//発射タイマーを初期化
			fireTimer = fireInterval;
		}

		break;
	case Phase::Back:
		//後退する
		position += velocity;
		//一定の位置まで後退したら前進
		if (position.z > 50.0f) {
			phase = Phase::Previous;
		}

		break;
	}
}
