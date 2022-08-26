#include "BossMainBody.h"
#include "Easing.h"
#include "GameScene.h"

GameScene* BossMainBody::gameScene = nullptr;
ObjModel* BossMainBody::bulletModel = nullptr;
const float BossMainBody::attackModeRotY = 180.0f;
const float BossMainBody::waitModeRotY = 0.0f;

BossMainBody* BossMainBody::Create(ObjModel* model, const Vector3& position)
{
	//ボス(本体)のインスタンスを生成
	BossMainBody* bossMainBody = new BossMainBody();
	if (bossMainBody == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	bossMainBody->model = model;

	// 初期化
	if (!bossMainBody->Initialize()) {
		delete bossMainBody;
		assert(0);
		return nullptr;
	}

	//座標をセット
	bossMainBody->position = position;

	//初期座標をセット
	bossMainBody->bornPos = position;

	//大きさをセット
	bossMainBody->scale = { 1.5f, 1.5f, 1.5f };

	return bossMainBody;
}

void BossMainBody::Damage(int damageNum)
{
	//引数の数字の分ダメージを喰らう
	HP -= damageNum;

	//HPが0以下になったら死亡
	if (HP <= 0) {
		isDead = true;
	}
}

void BossMainBody::Fall(const float time)
{
	//生成位置から降りたところで停止する
	Vector3 stayPos = bornPos;
	stayPos.y = 0;
	position = Easing::Lerp(bornPos, stayPos, time);
}

void BossMainBody::AttackTypeA(const Vector3& playerPosition)
{
	//プレイヤー方向に移動する
	Vector3 velocity = playerPosition - position;
	const float moveSpeed = 2.0f;
	velocity = velocity.normalize() * moveSpeed;
	position.x += velocity.x;
	position.y += velocity.y;

	//弾発射タイマーカウント
	fireTimer++;

	//弾発射を開始するか
	if (!isFire) {
		const int firstFireInterval = 200;
		if (fireTimer >= firstFireInterval) { isFire = true; }
		else { return; }
	}

	//発射間隔
	const int fireInterval = 10;
	if (fireTimer >= fireInterval) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = 0;
	}
}

void BossMainBody::AttackTypeB()
{
	//タイマーが移動する時間を越えていたら抜ける
	const float moveTime = 100;
	if (attackBTimer >= moveTime) { return; }

	//タイマーを更新
	attackBTimer++;
	const float time = attackBTimer / moveTime;

	//奥に移動させる
	const float moveNum = 30;
	position.z = Easing::OutQuad(bornPos.z, bornPos.z + moveNum, time);
}

void BossMainBody::AttackTypeC(const Vector3& playerPosition)
{
	//プレイヤー方向に移動する
	Vector3 velocity = playerPosition - position;
	const float moveSpeed = 1.0f;
	velocity = velocity.normalize() * moveSpeed;
	position.x += velocity.x;
	position.y += velocity.y;
}

void BossMainBody::ChangeAttackMode(const float time)
{
	//180度回転させて反対向きにする
	rotation.y = Easing::InOutBack(waitModeRotY, attackModeRotY, time);
}

void BossMainBody::ChangeWaitMode(const float time)
{
	//180度回転させて反対向きにする
	rotation.x = 0;
	rotation.y = Easing::InOutBack(attackModeRotY, waitModeRotY, time);
	rotation.z = 0;
}

void BossMainBody::ReturnFixedPosition(const float time)
{
	//座標(0, 0, Z)に戻す
	position = Easing::Lerp(returnStartPos, { 0, 0, bornPos.z }, time);
}

void BossMainBody::AttackEnd()
{
	//呼び出した瞬間の座標を固定位置に戻るときの出発座標として記録しておく
	returnStartPos = position;

	//弾発射タイマーを初期化
	fireTimer = 0;
	//弾発射状態を解除
	isFire = false;

	//攻撃内容Bの変数の初期化
	attackBTimer = 0;
}

Vector3 BossMainBody::GetWorldPos()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//平行移動成分を取得
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void BossMainBody::Fire()
{
	//弾の速度を設定
	const float bulletSpeed = 1.5f;
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, GetWorldPos(), velocity));
	gameScene->AddEnemyBullet(std::move(newBullet));
}
