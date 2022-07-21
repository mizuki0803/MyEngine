#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"

Player* Enemy::player = nullptr;
GameScene* Enemy::gameScene = nullptr;
ObjModel* Enemy::bulletModel = nullptr;

Enemy* Enemy::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
{
	//敵のインスタンスを生成
	Enemy* enemy = new Enemy();
	if (enemy == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	enemy->model = model;

	// 初期化
	if (!enemy->Initialize()) {
		delete enemy;
		assert(0);
		return nullptr;
	}


	//座標をセット
	enemy->position = position;

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

	PreviousPhaseInit();

	return true;
}

void Enemy::Update()
{
	//移動
	Move();

	//3Dオブジェクトの更新
	ObjObject3d::Update();
}

void Enemy::OnCollision()
{
}

void Enemy::PreviousPhaseInit()
{
	//発射タイマーを初期化
	fireTimer = fireInterval;
}

Vector3 Enemy::GetWorldPos()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//平行移動成分を取得
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
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
		if (position.z > 50.0f) {
			phase = Phase::Previous;
		}

		break;
	}
}

void Enemy::Fire()
{
	//弾の速度を設定
	const float bulletSpeed = 0.5f;

	Vector3 playerPos = player->GetWorldPos();
	Vector3 enemyPos = GetWorldPos();
	Vector3 DifferenceVec = playerPos - enemyPos;
	DifferenceVec.normalize();
	Vector3 velocity = DifferenceVec * bulletSpeed;

	//弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, position, velocity));
	gameScene->AddEnemyBullet(std::move(newBullet));
}
