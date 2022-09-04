#include "BossMainBody.h"
#include "Easing.h"
#include "GameScene.h"

void (BossMainBody::* BossMainBody::attackTypeTrackingPhaseFuncTable[])() = {
	&BossMainBody::AttackTypeTrackingLockon,
	&BossMainBody::AttackTypeTrackingShot,
	&BossMainBody::Stay,
};

void (BossMainBody::* BossMainBody::attackTypeSuperGiantBulletPhaseFuncTable[])() = {
	&BossMainBody::AttackTypeSuperGiantBulletMove,
	&BossMainBody::AttackTypeSuperGiantBulletChargeShot,
	&BossMainBody::Stay,
};

void (BossMainBody::* BossMainBody::attackTypeRotatePhaseFuncTable[])() = {
	&BossMainBody::AttackTypeRotateWait,
	&BossMainBody::AttackTypeRotateShot,
	&BossMainBody::AttackTypeRotateMove,
	&BossMainBody::Stay,
};

GameScene* BossMainBody::gameScene = nullptr;
ObjModel* BossMainBody::mainBodyModel = nullptr;
ObjModel* BossMainBody::mainBodySleepModel = nullptr;
ObjModel* BossMainBody::bulletModel = nullptr;
const float BossMainBody::attackModeRotY = 180.0f;
const float BossMainBody::waitModeRotY = 0.0f;

BossMainBody* BossMainBody::Create(const Vector3& basePos)
{
	//ボス(本体)のインスタンスを生成
	BossMainBody* bossMainBody = new BossMainBody();
	if (bossMainBody == nullptr) {
		return nullptr;
	}

	//寝ている状態のモデルをセット
	assert(mainBodySleepModel);
	bossMainBody->model = mainBodySleepModel;

	// 初期化
	if (!bossMainBody->Initialize()) {
		delete bossMainBody;
		assert(0);
		return nullptr;
	}

	//停止する基準の座標をセット
	bossMainBody->basePos = basePos;

	//大きさをセット
	bossMainBody->scale = { 4.5f, 4.5f, 4.5f };

	return bossMainBody;
}

void BossMainBody::Update()
{
	//ダメージ色状態のみの処理
	if (isDamageColor) {
		DamageColorMode();
	}

	//オブジェクト更新
	ObjObject3d::Update();
}

void BossMainBody::Damage(int attackPower)
{
	//引数の攻撃力をダメージ量にセット
	damageNum = attackPower;

	//ダメージを与える
	HP -= damageNum;

	//HPが0以下になったら死亡
	if (HP <= 0) {
		isDead = true;

		//HPゲージバグを起こさないようマイナス分を0に調整
		damageNum += HP;
	}

	//ダメージ色状態にする
	isDamageColor = true;
	const XMFLOAT4 damageColor = { 1, 0, 0, 1 };
	color = damageColor;
	//ダメージ色状態タイマー初期化
	damageColorTimer = 0;
}

void BossMainBody::FallMode(const float time)
{
	//基準の位置の真上から降りてくる
	Vector3 bornPos = basePos;
	const float fallNum = 70;
	bornPos.y = basePos.y + fallNum;
	position = Easing::Lerp(bornPos, basePos, time);
}

void BossMainBody::AttackTypeTracking(const Vector3& playerPosition)
{
	//常にプレイヤー方向に移動する
	Vector3 velocity = playerPosition - position;
	const float moveSpeed = 2.0f;
	velocity = velocity.normalize() * moveSpeed;
	position.x += velocity.x;
	position.y += velocity.y;

	//行動
	(this->*attackTypeTrackingPhaseFuncTable[static_cast<size_t>(attackTrackingPhase)])();
}

void BossMainBody::AttackTypeSuperGiantBullet()
{
	//行動
	(this->*attackTypeSuperGiantBulletPhaseFuncTable[static_cast<size_t>(attackSuperGiantBulletPhase)])();
}

void BossMainBody::AttackTypeRotate()
{
	//行動
	(this->*attackTypeRotatePhaseFuncTable[static_cast<size_t>(attackRotatePhase)])();
}

void BossMainBody::AttackTypeAvatarBodyBlow()
{
	//タイマーが移動する時間を越えていたら抜ける
	const float moveTime = 100;
	if (attackTimer >= moveTime) { return; }

	//タイマーを更新
	attackTimer++;
	const float time = attackTimer / moveTime;

	//奥に移動させる
	const float moveNum = 30;
	position.z = Easing::OutQuad(basePos.z, basePos.z + moveNum, time);
}

void BossMainBody::AttackTypeAvatarGatling(const Vector3& playerPosition)
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

	//攻撃タイマーを初期化
	attackTimer = 0;

	//攻撃内容:追従変数の初期化
	attackTrackingPhase = AttackTypeTrackingPhase::Lockon;

	//攻撃内容:超巨大弾の変数の初期化
	attackSuperGiantBulletPhase = AttackTypeSuperGiantBulletPhase::Move;

	//攻撃内容:回転の変数の初期化
	attackRotatePhase = AttackTypeRotatePhase::Wait;
	attackRotateShotCount = 0;
	attackRotateMoveBeforePos = {};
	attackRotateMpveAfterPos = {};
}

void BossMainBody::DeadMode()
{
	//X軸回転させる
	Vector3 rotVel = { -0.5f, 0, 0 };
	rotation += rotVel;
}

void BossMainBody::ChangeModel()
{
	//起きている状態のモデルをセット
	model = mainBodyModel;
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

void BossMainBody::DamageColorMode()
{
	//ダメージ色にする時間
	const float damageColorTime = 10;
	damageColorTimer++;

	//タイマーが指定した時間になったらダメージ色状態を解除する
	if (damageColorTimer >= damageColorTime) {
		//色を元に戻す
		isDamageColor = false;
		color = { 1,1,1,1 };
	}
}

void BossMainBody::AttackTypeTrackingLockon()
{
	//タイマーを更新
	const float lockonTime = 120;
	attackTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= lockonTime) {
		attackTrackingPhase = AttackTypeTrackingPhase::Shot;

		//タイマー初期化
		attackTimer = 0;
	}
}

void BossMainBody::AttackTypeTrackingShot()
{
	//タイマーを更新
	const float shotTime = 380;
	attackTimer++;

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
	if (attackTimer >= shotTime) {
		attackTrackingPhase = AttackTypeTrackingPhase::Stay;

		//タイマー初期化
		attackTimer = 0;
	}
}

void BossMainBody::AttackTypeSuperGiantBulletMove()
{
	//タイマーを更新
	const float moveTime = 150;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//奥に移動させる
	const float moveZ = 100;
	position.z = Easing::OutQuad(basePos.z, basePos.z + moveZ, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= moveTime) {
		attackSuperGiantBulletPhase = AttackTypeSuperGiantBulletPhase::ChargeShot;

		//タイマー初期化
		attackTimer = 0;
	}
}

void BossMainBody::AttackTypeSuperGiantBulletChargeShot()
{
	//タイマーを更新
	const float chargeTime = 300;
	attackTimer++;
	const float time = attackTimer / chargeTime;

	//色を赤くする
	color.y = Easing::OutQuad(1, 0, time);
	color.z = Easing::OutQuad(1, 0, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= chargeTime) {
		attackSuperGiantBulletPhase = AttackTypeSuperGiantBulletPhase::Stay;

		//タイマー初期化
		attackTimer = 0;

		//弾を発射
		const float bulletScale = 20.0f;
		const float bulletSpeed = 1.0f;
		Fire(bulletScale, bulletSpeed);

		//色を元に戻す
		color = { 1, 1, 1, 1 };
	}
}

void BossMainBody::AttackTypeRotateWait()
{
	//タイマーを更新
	const float waitTime = 60;
	attackTimer++;
	const float time = attackTimer / waitTime;

	//攻撃内容が分かりやすようにZ軸に一回転させる
	rotation.z = Easing::OutQuad(0, 360, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= waitTime) {
		attackRotatePhase = AttackTypeRotatePhase::Shot;

		//タイマー初期化
		attackTimer = 0;

		//念の為Z軸回転を0に戻しておく
		rotation.z = 0;
	}
}

void BossMainBody::AttackTypeRotateShot()
{
	//弾を発射
	const float bulletScale = 5.0f;
	const float bulletSpeed = 1.5f;
	Fire(bulletScale, bulletSpeed);

	//攻撃回数をカウント
	attackRotateShotCount++;

	//指定した回数攻撃していなければ次の位置に移動させる
	const int attackNum = 6;
	if (attackRotateShotCount < attackNum) {
		attackRotatePhase = AttackTypeRotatePhase::Move;

		//次の移動後の座標をセット
		if (attackRotateShotCount == 1) { attackRotateMpveAfterPos = { 10, 10, position.z }; }
		else if (attackRotateShotCount == 2) { attackRotateMpveAfterPos = { -10, 10, position.z }; }
		else if (attackRotateShotCount == 3) { attackRotateMpveAfterPos = { 10, -10, position.z }; }
		else if (attackRotateShotCount == 4) { attackRotateMpveAfterPos = { -10, -10, position.z }; }
		else if (attackRotateShotCount == 5) { attackRotateMpveAfterPos = { 0, 0, position.z }; }

		//現在の座標を移動前座標として記録しておく
		attackRotateMoveBeforePos = position;
	}
	//指定した回数攻撃したら待機状態にする
	else {
		attackRotatePhase = AttackTypeRotatePhase::Stay;
	}
}

void BossMainBody::AttackTypeRotateMove()
{
	//タイマーを更新
	const float moveTime = 60;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//指定した位置に移動させる
	const float moveZ = 100;
	position.x = Easing::OutQuad(attackRotateMoveBeforePos.x, attackRotateMpveAfterPos.x, time);
	position.y = Easing::OutQuad(attackRotateMoveBeforePos.y, attackRotateMpveAfterPos.y, time);

	//攻撃内容が分かりやすようにZ軸に一回転させる
	rotation.z = Easing::OutQuad(0, 360, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= moveTime) {
		attackRotatePhase = AttackTypeRotatePhase::Shot;

		//タイマー初期化
		attackTimer = 0;

		//念の為Z軸回転を0に戻しておく
		rotation.z = 0;
	}
}

void BossMainBody::Stay()
{
}
