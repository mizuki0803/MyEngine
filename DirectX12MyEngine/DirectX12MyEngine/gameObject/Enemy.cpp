#include "Enemy.h"

Enemy* Enemy::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
{
	//敵のインスタンスを生成
	Enemy* enemy = new Enemy();
	if (enemy == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	enemy->SetModel(model);

	// 初期化
	if (!enemy->Initialize()) {
		delete enemy;
		assert(0);
		return nullptr;
	}


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

	//弾発射
	//Fire();

	PreviousPhaseInit();

	return true;
}

void Enemy::Update()
{
	//死亡した弾の削除
	enemyBullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->GetIsDead();
		});

	//移動
	Move();

	//3Dオブジェクトの更新
	ObjObject3d::Update();

	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		bullet->Update();
	}
}

void Enemy::Draw()
{
	ObjObject3d::Draw();

	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		bullet->Draw();
	}
}

void Enemy::PreviousPhaseInit()
{
	//発射タイマーを初期化
	fireTimer = fireInterval;
}

void Enemy::Move()
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
		if (position.z > 100.0f) {
			phase = Phase::Previous;
		}

		break;
	}
}

void Enemy::Fire()
{
	//弾の速度を設定
	const float bulletSpeed = 0.5f;
	Vector3 velocity(0, 0, -bulletSpeed);

	//弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(model, position, velocity));
	enemyBullets.push_back(std::move(newBullet));
}
