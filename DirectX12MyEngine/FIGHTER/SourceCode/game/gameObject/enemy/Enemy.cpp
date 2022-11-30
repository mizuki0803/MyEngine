#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"
#include "EnemyDefeatCounter.h"
#include "ParticleEmitter.h"
#include "Easing.h"

Player* Enemy::player = nullptr;
GameScene* Enemy::gameScene = nullptr;
ObjModel* Enemy::bulletModel = nullptr;

bool Enemy::Initialize()
{
	//オブジェクト初期化
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//大きさ変更
	const float scaleNum = 2;
	scale = { scaleNum, scaleNum, scaleNum };

	//通常サイズをセット
	normalSize = scale;
	//ダメージ状態のサイズをセット
	damageSize = scale * 1.1f;

	//回転
	rotation.y = 180;

	return true;
}

void Enemy::Update()
{
	//当たり判定が作用したのは前フレームになるので、falseに戻しておく
	if (isCollisionFrame) {
		isCollisionFrame = false;
	}

	//ダメージ状態の処理
	if (isDamage) {
		DamageMode();
	}

	//オブジェクト更新
	ObjObject3d::Update();
}

void Enemy::OnCollision()
{
	//爆発用大きさ
	float explosionSize = 0.8f;

	//死亡状態でなければ死亡させる
	if (!isDead) {
		isDead = true;

		//ダメージ状態
		isDamage = true;

		//サイズを少し大きくする
		scale = damageSize;

		//倒した数カウンターを増やす
		EnemyDefeatCounter::AddCounter();
	}
	//既に死亡状態で死亡演出中のときは削除する
	else {
		isDelete = true;

		//被弾して削除するのでフラグを立てる
		isHitDelete = true;

		//破壊
		Break();

		//爆発のサイズを大きくする
		explosionSize = 1.6f;
	}

	//当たり判定がこのフレームで作用した
	isCollisionFrame = true;

	//爆発演出用パーティクル生成
	ParticleEmitter::GetInstance()->Explosion(position, explosionSize);
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

void Enemy::DamageMode()
{
	//ダメージ状態の時間
	const int damageTime = 20;
	damageTimer++;

	//大きくしたサイズを戻す
	DamageSizeReturn();

	//タイマーが指定した時間になったら
	if (damageTimer >= damageTime) {
		//ダメージ状態を終了
		isDamage = false;
	}
}

void Enemy::DamageSizeReturn()
{
	//大きくしたサイズを元に戻す時間
	const float sizeReturnTime = 10;
	//指定した時間以上なら抜ける
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//大きさを元に戻す
	scale = Easing::LerpVec3(damageSize, normalSize, time);
}

void Enemy::Break()
{
	//破壊用エフェクト追加
	for (int i = 0; i < 5; i++) {
		//ランダムでエフェクトの速度をセット
		const Vector3 randVel = { 2, 2, 2 };
		Vector3 velocity;
		velocity.x = (float)((rand() % (int)randVel.x) - randVel.x / 2);
		velocity.y = (float)((rand() % (int)randVel.y));
		velocity.z = (float)((rand() % (int)randVel.z) - randVel.z / 2);

		//値が大きいので割り算して小さくする
		const float div = 10;
		velocity /= div;

		const Vector3 scale = { 0.5f, 0.5f, 0.5f };
		BreakEffect(bulletModel, velocity, {}, scale);
	}
}

void Enemy::BreakEffect(ObjModel* model, const Vector3& velocity, const Vector3& rotSpeed, const Vector3& scale)
{
	//破壊用エフェクトを生成
	std::unique_ptr<EnemyBreakEffect> newBreakEffect;
	newBreakEffect.reset(EnemyBreakEffect::Create(model, position, velocity, rotSpeed, scale));
	gameScene->AddEnemyBreakEffect(std::move(newBreakEffect));
}

void Enemy::FrontOfScreenDelete()
{
	//座標が自機より手前(画面外手前)まで行ったら削除
	const float flontOfScreenDiffence = 50;
	const float deletePos = player->GetWorldPos().z - flontOfScreenDiffence;
	const Vector3 worldPos = GetWorldPos();

	if (worldPos.z <= deletePos) {
		isDelete = true;
	}
}
