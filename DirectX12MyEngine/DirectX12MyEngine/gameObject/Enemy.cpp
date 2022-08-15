#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"

Player* Enemy::player = nullptr;
GameScene* Enemy::gameScene = nullptr;
ObjModel* Enemy::bulletModel = nullptr;

void Enemy::OnCollision()
{
	//死亡させる
	isDead = true;
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

Vector2 Enemy::GetScreenPos()
{
	Vector3 worldPos = GetWorldPos();

	//ビューポート行列
	XMMATRIX matViewport = {
		{1280 / 2, 0, 0, 0 },
		{0, -720 / 2, 0, 0},
		{0, 0, 1, 0},
		{1280 / 2, 720 / 2, 0, 1}
	};

	//ビュー行列とプロジェクション行列とビューポート行列を合成
	XMMATRIX matViewProjectionViewport =
		camera->GetMatView() * camera->GetMatProjection() * matViewport;

	//ワールド座標からスクリーン座標に変換
	worldPos = MatrixTransformWDivision(worldPos, matViewProjectionViewport);
	Vector2 screenPos = { worldPos.x, worldPos.y };

	return screenPos;
}

void Enemy::Fire()
{
	//座標が自機より手前なら発射しない
	if (GetWorldPos().z <= player->GetWorldPos().z) { return; }

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

void Enemy::FrontOfScreenDelete()
{
	//座標が自機より手前(画面外手前)まで行ったら削除
	const float flontOfScreenDiffence = 50;
	const float deletePos = player->GetWorldPos().z - flontOfScreenDiffence;
	const Vector3 worldPos = GetWorldPos();

	if (worldPos.z <= deletePos) {
		isDead = true;
	}
}
