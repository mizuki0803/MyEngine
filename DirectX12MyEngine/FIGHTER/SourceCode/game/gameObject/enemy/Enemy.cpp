#include "Enemy.h"
#include "BasePlayer.h"
#include "BaseGameCamera.h"
#include "BaseStageScene.h"
#include "EnemyDefeatCounter.h"
#include "ParticleEmitter.h"
#include "Easing.h"

BasePlayer* Enemy::player = nullptr;
BaseGameCamera* Enemy::gameCamera = nullptr;
BaseStageScene* Enemy::stageScene = nullptr;
ObjModel* Enemy::bulletModel = nullptr;
bool Enemy::isGroundMode = true;
const XMFLOAT4 Enemy::damageColor = { 1, 0.2f, 0.2f, 1 };

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
	damageSize = scale * 1.05f;

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

void Enemy::OnCollision(const int damageNum)
{
	//爆発用大きさ
	float explosionSize = 1.5f;

	//死亡状態でなければダメージを喰らわせる
	if (!isDead) {
		//HPを受けたダメージ量の分減らす
		HP -= damageNum;
		//ダメージ状態にする
		isDamage = true;
		//ダメージ状態タイマー初期化
		damageTimer = 0;
		//色を変更
		color = damageColor;

		//サイズを少し大きくする
		scale = damageSize;

		//HPが0以下になったら死亡させる
		if (HP <= 0) {
			isDead = true;

			//倒した数カウンターを増やす
			EnemyDefeatCounter::AddCounter();
		}
	}
	//既に死亡状態で死亡演出中のときは削除する
	else {
		isDelete = true;

		//被弾して削除するのでフラグを立てる
		isHitDelete = true;

		//破壊
		Break();

		//爆発のサイズを大きくする
		explosionSize = 2.5f;
	}

	//当たり判定がこのフレームで作用した
	isCollisionFrame = true;

	//爆発演出用パーティクル生成
	ParticleEmitter::GetInstance()->Explosion(position, explosionSize);
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

void Enemy::FireLookAngle()
{
	//座標が自機より手前なら発射しない
	if (GetWorldPos().z <= player->GetWorldPos().z) { return; }

	//弾の速度を設定
	const float bulletSpeed = 1.0f;

	//向いている方向に弾を発射
	Vector3 velocity = { 0, 0, bulletSpeed };
	velocity = MatrixTransformDirection(velocity, matWorld);

	//弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, position, velocity));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void Enemy::FirePlayerAngle()
{
	//自機がセットされていなければ抜ける
	if (!player) { return; }
	//座標が自機より手前なら発射しない
	if (GetWorldPos().z <= player->GetWorldPos().z) { return; }

	//弾の速度を設定
	const float bulletSpeed = 1.0f;

	Vector3 playerPos = player->GetWorldPos();
	Vector3 enemyPos = GetWorldPos();
	Vector3 DifferenceVec = playerPos - enemyPos;
	DifferenceVec.normalize();
	Vector3 velocity = DifferenceVec * bulletSpeed;

	//弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, position, velocity));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void Enemy::DamageMode()
{
	//ダメージ状態の時間
	const int damageTime = 20;
	damageTimer++;

	//大きくしたサイズを戻す
	DamageSizeReturn();

	//ダメージ色切り替え
	DamageColorMode();

	//タイマーが指定した時間になったら
	if (damageTimer >= damageTime) {
		//ダメージ状態を終了
		isDamage = false;

		//色を元に戻しておく
		color = { 1, 1, 1, 1 };
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

void Enemy::DamageColorMode()
{
	//ダメージ色切り替え時間
	const int damageColorChangeTime = 2;

	//タイマーが指定した時間になったら
	if (damageTimer % damageColorChangeTime == 0) {
		//ダメージ色状態を切り替える
		if (isDamageColor) {
			isDamageColor = false;

			//色を元に戻す
			color = { 1, 1, 1, 1 };
		}
		else {
			isDamageColor = true;

			//ダメージ色にする
			color = damageColor;
		}
	}
}

void Enemy::DeadSmokeEffect(const float size, const int smokeStartTime)
{
	//死亡タイマー更新
	deadTimer++;

	//タイマーが黒煙を出すまでの時間以下なら抜ける
	if (deadTimer < smokeStartTime) { return; }

	//毎フレーム出すと多いので間隔を設定
	const int smokeInterval = 5;
	//指定した間隔以外なら抜ける
	if (deadTimer % smokeInterval != 0) { return; }

	//黒煙パーティクル生成
	ParticleEmitter::GetInstance()->BlackSmoke(position, size);
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
	stageScene->AddEnemyBreakEffect(std::move(newBreakEffect));
}

void Enemy::FrontOfScreenDelete()
{
	//ゲームカメラがセットされていなければ抜ける
	if (!gameCamera) { return; }

	//座標がカメラより手前(画面外手前)まで行ったら削除
	const float flontOfScreenDiffence = 20;
	const float deletePos = gameCamera->GetPosition().z - flontOfScreenDiffence;
	if (position.z <= deletePos) {
		isDelete = true;
	}
}
