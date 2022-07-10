#include "Enemy.h"

Enemy* Enemy::Create(ObjModel* model, const XMFLOAT3& position, const XMFLOAT3& velocity)
{
	//敵のインスタンスを生成
	Enemy* enemy = new Enemy();
	if (enemy == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!enemy->Initialize()) {
		delete enemy;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(model);
	enemy->SetModel(model);

	//座標をセット
	enemy->SetPosition(position);

	//速度をセット
	enemy->velocity = velocity;

	return enemy;
}

bool Enemy::Initialize()
{
	//3Dオブジェクトの初期化
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	return true;
}

void Enemy::Update()
{
	//移動
	Move();

	//3Dオブジェクトの更新
	ObjObject3d::Update();
}

void Enemy::Move()
{
	switch (phase)
	{
	case Phase::Previous:
	default:
		//前進する
		position.x += velocity.x;
		position.y += velocity.y;
		position.z += velocity.z;
		//一定の位置まで前進したら後退
		if (position.z < 0.0f) {
			phase = Phase::Back;
		}

		break;
	case Phase::Back:
		//後退する
		position.x -= velocity.x;
		position.y -= velocity.y;
		position.z -= velocity.z;
		//一定の位置まで後退したら前進
		if (position.z > 100.0f) {
			phase = Phase::Previous;
		}

		break;
	}
}
