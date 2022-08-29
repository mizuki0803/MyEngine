#include "BossMainBody.h"
#include "Easing.h"
#include "GameScene.h"

void (BossMainBody::* BossMainBody::attackTypeAPhaseFuncTable[])() = {
	&BossMainBody::AttackTypeALockon,
	&BossMainBody::AttackTypeAShot,
	&BossMainBody::Stay,
};

void (BossMainBody::* BossMainBody::attackTypeA2PhaseFuncTable[])() = {
	&BossMainBody::AttackTypeA2Move,
	&BossMainBody::AttackTypeA2ChargeShot,
	&BossMainBody::Stay,
};

void (BossMainBody::* BossMainBody::attackTypeA3PhaseFuncTable[])() = {
	&BossMainBody::AttackTypeA3Wait,
	&BossMainBody::AttackTypeA3Shot,
	&BossMainBody::AttackTypeA3Move,
	&BossMainBody::Stay,
};

GameScene* BossMainBody::gameScene = nullptr;
ObjModel* BossMainBody::bulletModel = nullptr;
const float BossMainBody::attackModeRotY = 180.0f;
const float BossMainBody::waitModeRotY = 0.0f;

BossMainBody* BossMainBody::Create(ObjModel* model, const Vector3& basePos)
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

	//停止する基準の座標をセット
	bossMainBody->basePos = basePos;

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
	//基準の位置の真上から降りてくる
	Vector3 bornPos = basePos;
	const float fallNum = 70;
	bornPos.y = basePos.y + fallNum;
	position = Easing::Lerp(bornPos, basePos, time);
}

void BossMainBody::AttackTypeA(const Vector3& playerPosition)
{
	//常にプレイヤー方向に移動する
	Vector3 velocity = playerPosition - position;
	const float moveSpeed = 2.0f;
	velocity = velocity.normalize() * moveSpeed;
	position.x += velocity.x;
	position.y += velocity.y;

	//行動
	(this->*attackTypeAPhaseFuncTable[static_cast<size_t>(attackAPhase)])();
}

void BossMainBody::AttackTypeA2()
{
	//行動
	(this->*attackTypeA2PhaseFuncTable[static_cast<size_t>(attackA2Phase)])();
}

void BossMainBody::AttackTypeA3()
{
	//行動
	(this->*attackTypeA3PhaseFuncTable[static_cast<size_t>(attackA3Phase)])();
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
	position.z = Easing::OutQuad(basePos.z, basePos.z + moveNum, time);
}

void BossMainBody::AttackTypeC(const Vector3& playerPosition)
{
	//常にプレイヤー方向に移動する
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

void BossMainBody::ReturnBasePosition(const float time)
{
	//基準の座標に戻す
	position = Easing::Lerp(returnStartPos, basePos, time);
}

void BossMainBody::AttackEnd()
{
	//呼び出した瞬間の座標を基準位置に戻るときの出発座標として記録しておく
	returnStartPos = position;

	//弾発射タイマーを初期化
	fireTimer = 0;

	//攻撃内容Aの変数の初期化
	attackAPhase = AttackTypeAPhase::Lockon;
	attackATimer = 0;

	//攻撃内容A2の変数の初期化
	attackA2Phase = AttackTypeA2Phase::Move;
	attackA2Timer = 0;

	//攻撃内容A3の変数の初期化
	attackA3Phase = AttackTypeA3Phase::Wait;
	attackA3Timer = 0;
	attackA3ShotCount = 0;
	attackA3MoveBeforePos = {};
	attackA3MpveAfterPos = {};

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

void BossMainBody::Fire(const float scale, const float bulletSpeed)
{
	//弾の速度を設定
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, GetWorldPos(), velocity, scale));
	gameScene->AddEnemyBullet(std::move(newBullet));
}

void BossMainBody::AttackTypeALockon()
{
	//タイマーを更新
	const float lockonTime = 120;
	attackATimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackATimer >= lockonTime) {
		attackAPhase = AttackTypeAPhase::Shot;

		//タイマー初期化
		attackATimer = 0;
	}
}

void BossMainBody::AttackTypeAShot()
{
	//タイマーを更新
	const float shotTime = 380;
	attackATimer++;

	//弾発射タイマーカウント
	fireTimer++;
	//発射間隔
	const int fireInterval = 10;
	if (fireTimer >= fireInterval) {
		//弾を発射
		const float bulletScale = 1.0f;
		const float bulletSpeed = 1.5f;
		Fire(bulletScale, bulletSpeed);
		//発射タイマーを初期化
		fireTimer = 0;
	}

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackATimer >= shotTime) {
		attackAPhase = AttackTypeAPhase::Stay;

		//タイマー初期化
		attackATimer = 0;
	}
}

void BossMainBody::AttackTypeA2Move()
{
	//タイマーを更新
	const float moveTime = 150;
	attackA2Timer++;
	const float time = attackA2Timer / moveTime;

	//奥に移動させる
	const float moveZ = 100;
	position.z = Easing::OutQuad(basePos.z, basePos.z + moveZ, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackA2Timer >= moveTime) {
		attackA2Phase = AttackTypeA2Phase::ChargeShot;

		//タイマー初期化
		attackA2Timer = 0;
	}
}

void BossMainBody::AttackTypeA2ChargeShot()
{
	//タイマーを更新
	const float chargeTime = 300;
	attackA2Timer++;
	const float time = attackA2Timer / chargeTime;

	//色を赤くする
	color.y = Easing::OutQuad(1, 0, time);
	color.z = Easing::OutQuad(1, 0, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackA2Timer >= chargeTime) {
		attackA2Phase = AttackTypeA2Phase::Stay;

		//タイマー初期化
		attackA2Timer = 0;

		//弾を発射
		const float bulletScale = 20.0f;
		const float bulletSpeed = 1.0f;
		Fire(bulletScale, bulletSpeed);

		//色を元に戻す
		color = { 1, 1, 1, 1 };
	}
}

void BossMainBody::AttackTypeA3Wait()
{
	//タイマーを更新
	const float waitTime = 60;
	attackA3Timer++;
	const float time = attackA3Timer / waitTime;

	//攻撃内容が分かりやすようにZ軸に一回転させる
	rotation.z = Easing::OutQuad(0, 360, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackA3Timer >= waitTime) {
		attackA3Phase = AttackTypeA3Phase::Shot;

		//タイマー初期化
		attackA3Timer = 0;

		//念の為Z軸回転を0に戻しておく
		rotation.z = 0;
	}
}

void BossMainBody::AttackTypeA3Shot()
{
	//弾を発射
	const float bulletScale = 5.0f;
	const float bulletSpeed = 1.5f;
	Fire(bulletScale, bulletSpeed);

	//攻撃回数をカウント
	attackA3ShotCount++;

	//指定した回数攻撃していなければ次の位置に移動させる
	const int attackNum = 6;
	if (attackA3ShotCount < attackNum) {
		attackA3Phase = AttackTypeA3Phase::Move;

		//次の移動後の座標をセット
		if (attackA3ShotCount == 1) { attackA3MpveAfterPos = { 10, 10, position.z }; }
		else if (attackA3ShotCount == 2) { attackA3MpveAfterPos = { -10, 10, position.z }; }
		else if (attackA3ShotCount == 3) { attackA3MpveAfterPos = { 10, -10, position.z }; }
		else if (attackA3ShotCount == 4) { attackA3MpveAfterPos = { -10, -10, position.z }; }
		else if (attackA3ShotCount == 5) { attackA3MpveAfterPos = { 0, 0, position.z }; }

		//現在の座標を移動前座標として記録しておく
		attackA3MoveBeforePos = position;
	}
	//指定した回数攻撃したら待機状態にする
	else {
		attackA3Phase = AttackTypeA3Phase::Stay;
	}
}

void BossMainBody::AttackTypeA3Move()
{
	//タイマーを更新
	const float moveTime = 60;
	attackA3Timer++;
	const float time = attackA3Timer / moveTime;

	//指定した位置に移動させる
	const float moveZ = 100;
	position.x = Easing::OutQuad(attackA3MoveBeforePos.x, attackA3MpveAfterPos.x, time);
	position.y = Easing::OutQuad(attackA3MoveBeforePos.y, attackA3MpveAfterPos.y, time);

	//攻撃内容が分かりやすようにZ軸に一回転させる
	rotation.z = Easing::OutQuad(0, 360, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackA3Timer >= moveTime) {
		attackA3Phase = AttackTypeA3Phase::Shot;

		//タイマー初期化
		attackA3Timer = 0;

		//念の為Z軸回転を0に戻しておく
		rotation.z = 0;
	}
}

void BossMainBody::Stay()
{
}
