#include "BossMainBody.h"
#include "Easing.h"
#include "GameScene.h"
#include "BossChargeBullet.h"
#include "ParticleEmitter.h"

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
ObjModel* BossMainBody::mainBodyDamageModel = nullptr;
ObjModel* BossMainBody::mainBodySleepModel = nullptr;
ObjModel* BossMainBody::mainBodyDeadModel = nullptr;
ObjModel* BossMainBody::bulletModel = nullptr;
const Vector3 BossMainBody::normalSize = { 4.5f, 4.5f, 4.5f };
const Vector3 BossMainBody::damageSize = BossMainBody::normalSize * 1.1f;
const float BossMainBody::attackModeRotY = 180.0f;
const float BossMainBody::waitModeRotY = 0.0f;
const XMFLOAT4 BossMainBody::damageColor = { 1, 0.2f, 0.2f, 1 };

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
	bossMainBody->scale = normalSize;

	return bossMainBody;
}

void BossMainBody::Update()
{
	//ダメージ状態のみの処理
	if (isDamage) {
		DamageMode();
	}

	//オブジェクト更新
	ObjObject3d::Update();
}

void BossMainBody::Damage(int attackPower, const Vector3& collisionPos, const Vector3& subjectVel)
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

		//死亡時の座標と角度を記憶しておく
		deadPos = position;
		deadRota = rotation;
	}

	//HPが少ない状態のモデルをセットする
	DamageModelChange();

	//ダメージ状態にする
	isDamage = true;
	//ダメージを喰らった瞬間なのでtrue
	isDamageTrigger = true;
	//ダメージ状態タイマー初期化
	damageTimer = 0;
	//色を変更
	color = damageColor;

	//サイズを少し大きくする
	scale = damageSize;

	//ノックバック情報をセット
	SetDamageKnockback(subjectVel);

	//爆発生成する
	DamageExplosion(collisionPos);
}

void BossMainBody::DamageModelChange()
{
	//既にHPが少ない状態のモデルがセットされていたら抜ける
	if (isDamageModel) { return; }

	//HPが指定した値以下でないなら抜ける
	const int modelChangeHP = maxHP / 2;
	if (!(HP <= modelChangeHP)) { return; }

	//HPが少ない状態のモデルをセットする
	model = mainBodyDamageModel;
	//HPが少ない状態のモデルがセットされている状態にする
	isDamageModel = true;
}

void BossMainBody::Fall(const float time)
{
	//基準の位置の真上から降りてくる
	Vector3 bornPos = basePos;
	const float fallNum = 70;
	bornPos.y = basePos.y + fallNum;
	position = Easing::LerpVec3(bornPos, basePos, time);
}

void BossMainBody::UpdateBulletShotPos()
{
	//中心座標からの距離
	const Vector3 distancePos = { 0, 0, scale.z - 1.0f };
	//平行移動行列の計算
	XMMATRIX matTrans = XMMatrixTranslation(distancePos.x, distancePos.y, distancePos.z);

	//ワールド行列の合成
	XMMATRIX bulletShotMatWorld;
	bulletShotMatWorld = XMMatrixIdentity();	//変形をリセット
	bulletShotMatWorld *= matTrans;	//ワールド行列に平行移動を反映

	//自機オブジェクトのワールド行列をかける
	bulletShotMatWorld *= matWorld;

	//弾発射座標を取得
	bulletShotPos = { bulletShotMatWorld.r[3].m128_f32[0], bulletShotMatWorld.r[3].m128_f32[1], bulletShotMatWorld.r[3].m128_f32[2] };
}

void BossMainBody::AttackTypeTracking(const Vector3& playerPosition)
{
	//常にプレイヤー方向に移動する
	Vector3 velocity = playerPosition - position;
	const float moveSpeed = 2.0f;
	velocity = velocity.normalize() * moveSpeed;
	position.x += velocity.x;
	position.y += velocity.y;

	//地面にめり込まないようにする
	position.y = max(position.y, scale.y);

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

	//地面にめり込まないように調整
	const float moveLimitGround = 8.5f;
	position.y = max(position.y, moveLimitGround);
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

void BossMainBody::SetReturnBasePosition()
{
	//呼び出した瞬間の座標を基準位置に戻るときの出発座標として記録しておく
	returnStartPos = position;
}

void BossMainBody::ReturnBasePosition(const float time)
{
	//基準の座標に戻す
	position = Easing::LerpVec3(returnStartPos, basePos, time);
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

void BossMainBody::DeadBasePosBack(const float time)
{
	//イージング用比率が1を越えていたら抜ける
	if (time > 1.0f) { return; }

	//イージングで座標と角度を基準に戻す
	position.x = Easing::OutQuad(deadPos.x, basePos.x, time);
	position.y = Easing::OutQuad(deadPos.y, basePos.y, time);
	position.z = Easing::OutQuad(deadPos.z, basePos.z, time);

	rotation.x = Easing::OutQuad(deadRota.x, 0, time);
	rotation.y = Easing::OutQuad(deadRota.y, 0, time);
	rotation.z = Easing::OutQuad(deadRota.z, 0, time);
}

void BossMainBody::DeadFall()
{
	//X軸回転をしながら墜落する
	const float rotSpeed = 0.5f;
	rotation.x += rotSpeed;

	//墜落するため、速度に加速度を加える
	Vector3 crashAccel = { 0, -0.0025f, 0 };
	deadFallVel += crashAccel;
	//落下する速度の最大値を設定
	const float maxCrashSpeed = -0.15f;
	if (deadFallVel.y <= maxCrashSpeed) { deadFallVel.y = maxCrashSpeed; }
	position += deadFallVel;

	//爆発させる
	DeadFallExplosion();

	//Y座標が0以下になったら削除
	if (GetWorldPos().y <= 0) {
		isDelete = true;
	}
}

void BossMainBody::ChangeModel()
{
	//起きている状態のモデルをセット
	model = mainBodyModel;
}

void BossMainBody::ChangeDeadModel()
{
	//死亡用モデルに変更
	model = mainBodyDeadModel;
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
	newBullet.reset(EnemyBullet::Create(bulletModel, bulletShotPos, velocity, scale));
	gameScene->AddEnemyBullet(std::move(newBullet));
}

void BossMainBody::ChargeBulletFire(const float scale, const float bulletSpeed)
{
	//弾の速度を設定
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//チャージ弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(BossChargeBullet::Create(bulletShotPos, velocity, scale));
	gameScene->AddEnemyBullet(std::move(newBullet));
}

void BossMainBody::DamageMode()
{
	//ダメージ状態の時間
	const int damageTime = 20;
	damageTimer++;

	//ノックバック
	DamageKnockback();

	//大きくしたサイズを戻す
	DamageSizeReturn();

	//ダメージ色切り替え
	DamageColorMode();

	//ダメージトリガーフラグがtrueなら下ろしておく
	if (isDamageTrigger) { isDamageTrigger = false; }

	//タイマーが指定した時間になったら
	if (damageTimer >= damageTime) {
		//ダメージ状態を終了
		isDamage = false;

		//色を元に戻しておく
		color = { 1, 1, 1, 1 };
	}
}

void BossMainBody::SetDamageKnockback(const Vector3& subjectVel)
{
	//ノックバックする方向を決める(対象の速度の方向)
	knockbackVec = subjectVel;
	knockbackVec.normalize();
}

void BossMainBody::DamageKnockback()
{
	//ノックバックする時間
	const float knockbackTime = 5;
	//指定した時間以上なら抜ける
	if (damageTimer > knockbackTime) { return; }

	const float time = damageTimer / knockbackTime;

	//速度を作成
	const float knockbackBaseSpeed = 0.2f;
	knockbackVel = knockbackVec * (1 - time) * knockbackBaseSpeed;

	//自機をノックバックさせる
	position += knockbackVel;
}

void BossMainBody::DamageSizeReturn()
{
	//大きくしたサイズを元に戻す時間
	const float sizeReturnTime = 10;
	//指定した時間以上なら抜ける
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//大きさを元に戻す
	scale = Easing::LerpVec3(damageSize, normalSize, time);
}

void BossMainBody::DamageExplosion(const Vector3& position)
{
	//敵内部に演出が出てしまうことがあるので、敵の大きさ分押し戻す
	Vector3 pos = position;
	pos.z -= scale.z / 2;
	//ランダムでずらす
	const Vector3 randPos = { 2, 2, 1 };
	pos.x += (float)((rand() % (int)randPos.x) - randPos.x / 2);
	pos.y += (float)((rand() % (int)randPos.y) - randPos.y / 2);
	pos.z += (float)((rand() % (int)randPos.z));

	//ショット死亡演出用パーティクル生成
	const float size = 0.75f;
	ParticleEmitter::GetInstance()->Explosion(pos, size);
}

void BossMainBody::DamageColorMode()
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
		const float bulletScale = 1.5f;
		const float bulletSpeed = 1.2f;
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

	//発射する弾の大きさ
	const float bulletScale = 20.0f;
	//チャージ演出
	const float effectScale = Easing::OutCubic(0, bulletScale, time);
	ParticleEmitter::GetInstance()->BossCharge(bulletShotPos, effectScale);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= chargeTime) {
		attackSuperGiantBulletPhase = AttackTypeSuperGiantBulletPhase::Stay;

		//タイマー初期化
		attackTimer = 0;

		//弾を発射
		const float bulletSpeed = 1.0f;
		ChargeBulletFire(bulletScale, bulletSpeed);

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
	const float bulletSpeed = 1.0f;
	Fire(bulletScale, bulletSpeed);

	//攻撃回数をカウント
	attackRotateShotCount++;

	//指定した回数攻撃していなければ次の位置に移動させる
	const int attackNum = 6;
	if (attackRotateShotCount < attackNum) {
		attackRotatePhase = AttackTypeRotatePhase::Move;

		//次の移動後の座標をセット
		if (attackRotateShotCount == 1) { attackRotateMpveAfterPos = { basePos.x + 10, basePos.y + 10, position.z }; }
		else if (attackRotateShotCount == 2) { attackRotateMpveAfterPos = { basePos.x - 10, basePos.y + 10, position.z }; }
		else if (attackRotateShotCount == 3) { attackRotateMpveAfterPos = { basePos.x + 10, basePos.y - 10, position.z }; }
		else if (attackRotateShotCount == 4) { attackRotateMpveAfterPos = { basePos.x - 10, basePos.y - 10, position.z }; }
		else if (attackRotateShotCount == 5) { attackRotateMpveAfterPos = { basePos.x, basePos.y, position.z }; }

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

void BossMainBody::DeadFallExplosion()
{
	//爆発させる間隔の時間
	const int explosionInterval = 3;
	//タイマーを更新
	deadFallExplosionTimer++;

	//指定した間隔以外なら抜ける
	if (deadFallExplosionTimer % explosionInterval != 0) { return; }

	//爆発
	//敵内部に演出が出てしまうことがあるので、敵の大きさ分押し戻す
	Vector3 pos = position;
	pos.z -= scale.z / 2;
	//ランダムでずらす
	const Vector3 randPos = { 2, 2, 1 };
	pos.x += (float)((rand() % (int)randPos.x) - randPos.x / 2);
	pos.y += (float)((rand() % (int)randPos.y) - randPos.y / 2);
	pos.z += (float)((rand() % (int)randPos.z));

	//死亡爆発演出用パーティクル生成
	const float size = 1.0f;
	ParticleEmitter::GetInstance()->Explosion(pos, size);
}

void BossMainBody::Stay()
{
}
