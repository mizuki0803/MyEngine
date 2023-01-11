#include "GalaxyCannon.h"
#include "Easing.h"
#include "BaseStageScene.h"
#include "BossChargeBullet.h"
#include "ParticleEmitter.h"
#include "EnemyBreakEffect.h"

void (GalaxyCannon::* GalaxyCannon::attackTypeRapidFirePhaseFuncTable[])() = {
	&GalaxyCannon::AttackTypeRapidFireStartWait,
	&GalaxyCannon::AttackTypeRapidFireCharge,
	&GalaxyCannon::AttackTypeRapidFireShot,
	&GalaxyCannon::AttackTypeRapidFireReturn,
	&GalaxyCannon::AttackTypeRapidFireWait,
};

BaseStageScene* GalaxyCannon::stageScene = nullptr;
ObjModel* GalaxyCannon::cannonModel = nullptr;
ObjModel* GalaxyCannon::cannonDeadModel = nullptr;
ObjModel* GalaxyCannon::bulletModel = nullptr;
std::array<ObjModel*, 5> GalaxyCannon::breakModels;
const Vector3 GalaxyCannon::normalSize = { 0.22f, 0.22f, 0.22f };
const Vector3 GalaxyCannon::damageSize = normalSize * 1.1f;
const XMFLOAT4 GalaxyCannon::damageColor = { 1, 0.2f, 0.2f, 1 };

GalaxyCannon* GalaxyCannon::Create(ObjObject3d* parent, const Vector3& basePos)
{
	//ギャラクシー(大砲)のインスタンスを生成
	GalaxyCannon* galaxyCannon = new GalaxyCannon();
	if (galaxyCannon == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(cannonModel);
	galaxyCannon->model = cannonModel;

	// 初期化
	if (!galaxyCannon->Initialize()) {
		delete galaxyCannon;
		assert(0);
		return nullptr;
	}

	//親をセット
	galaxyCannon->parent = parent;
	//基準座標をセット
	galaxyCannon->basePos = basePos;
	//座標をセット
	galaxyCannon->position = basePos;
	//大きさをセット
	galaxyCannon->scale = normalSize;
	//角度をセット(右を向かせる)
	galaxyCannon->rotation = { 0, 270, 0 };

	return galaxyCannon;
}

void GalaxyCannon::SetBreakModel(int modelNum, ObjModel* model)
{
	//破壊時のモデルをセット
	assert(model);
	breakModels[modelNum] = model;
}

void GalaxyCannon::Update()
{
	//ダメージ状態の処理
	if (isDamage) {
		DamageMode();
	}

	//オブジェクト更新
	ObjObject3d::Update();
}

void GalaxyCannon::Damage(int attackPower, const Vector3& collisionPos)
{
	//引数の攻撃力をダメージ量にセット
	damageNum = attackPower;

	//ダメージを与える
	HP -= damageNum;

	//HPが0以下になったら死亡
	if (HP <= 0) {
		//死亡状態にする
		isDead = true;

		//破壊用演出を追加
		Break();
		
		//基準座標に戻す
		position = basePos;

		//HPゲージバグを起こさないようマイナス分を0に調整
		damageNum += HP;

		//死亡用モデルに変更
		model = cannonDeadModel;

		//爆発演出用パーティクル生成
		const float size = 5.0f;
		ParticleEmitter::GetInstance()->Explosion(GetWorldPos(), size);
	}

	//ダメージ状態にする
	isDamage = true;
	//ダメージを喰らった瞬間なのでtrue
	isDamageTrigger = true;
	//ダメージ状態タイマー初期化
	damageTimer = 0;
	//ダメージ状態色に変更
	color = damageColor;

	//サイズを少し大きくする
	scale = damageSize;

	//爆発生成する
	DamageExplosion(collisionPos);
}

void GalaxyCannon::UpdateBulletShotPos()
{
	//中心座標からの距離
	const Vector3 distancePos = { 0, 0, scale.z * parent->GetScale().z - 1.0f };

	//弾発射座標を取得
	bulletShotPos = LocalTranslation(distancePos, matWorld);
}

void GalaxyCannon::AttackModeStart()
{
	//攻撃内容:速射の変数の初期化
	attackTypeRapidFirePhase = AttackTypeRapidFirePhase::StartWait;
	rapidFireCount = 0;

	//攻撃で使うタイマーを初期化
	attackTimer = 0;

	//攻撃中にする
	isAttack = true;
}

void GalaxyCannon::AttackTypeRapidFire()
{
	//攻撃内容:速射行動
	(this->*attackTypeRapidFirePhaseFuncTable[static_cast<size_t>(attackTypeRapidFirePhase)])();
}

void GalaxyCannon::Fire(const float scale, const float bulletSpeed)
{
	//弾の速度を設定
	Vector3 velocity(0, 0, -bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, bulletShotPos, velocity, scale));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void GalaxyCannon::ChargeBulletFire(const float scale, const float bulletSpeed)
{
	//弾の速度を設定
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//チャージ弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(BossChargeBullet::Create(bulletShotPos, velocity, scale));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void GalaxyCannon::DamageMode()
{
	//ダメージ状態の時間
	const int damageTime = 20;
	damageTimer++;

	//大きくしたサイズを戻す
	DamageSizeReturn();

	//ダメージ色切り替え
	DamageColorChange();

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

void GalaxyCannon::DamageSizeReturn()
{
	//大きくしたサイズを元に戻す時間
	const float sizeReturnTime = 10;
	//指定した時間以上なら抜ける
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//大きさを元に戻す
	scale = Easing::LerpVec3(damageSize, normalSize, time);
}

void GalaxyCannon::DamageExplosion(const Vector3& collisionPos)
{
	//敵内部に演出が出てしまうことがあるので、敵の大きさ分押し戻す
	Vector3 pos = collisionPos;
	float avatarSize = scale.z;
	//親がいるときは親のスケールをかける
	if (parent) { avatarSize *= parent->GetScale().z; }
	pos.z -= avatarSize / 2;
	//ランダムでずらす
	const Vector3 randPos = { 2, 2, 1 };
	pos.x += (float)((rand() % (int)randPos.x) - randPos.x / 2);
	pos.y += (float)((rand() % (int)randPos.y) - randPos.y / 2);
	pos.z += (float)((rand() % (int)randPos.z));

	//爆発演出用パーティクル生成
	const float size = 0.75f;
	ParticleEmitter::GetInstance()->Explosion(pos, size);
}

void GalaxyCannon::DamageColorChange()
{
	//ダメージ色切り替え間隔時間
	const int colorChangeInterval = 2;

	//タイマーが指定した間隔以外なら抜ける
	if (damageTimer % colorChangeInterval != 0) { return; }

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

void GalaxyCannon::AttackTypeRapidFireStartWait()
{
	//タイマー更新
	attackTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= rapidFireStartWaitTime) {
		attackTypeRapidFirePhase = AttackTypeRapidFirePhase::Charge;

		//タイマーを初期化しておく
		attackTimer = 0;
	}
}

void GalaxyCannon::AttackTypeRapidFireCharge()
{
	//チャージにかかる時間
	const float chargeTime = 50;
	//タイマー更新
	attackTimer++;
	const float time = attackTimer / chargeTime;

	//イージングで後ろに引っ張る
	const float easeStart = basePos.x;
	const float easeEnd = basePos.x - 0.3f;
	position.x = Easing::OutQuint(easeStart, easeEnd, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= chargeTime) {
		attackTypeRapidFirePhase = AttackTypeRapidFirePhase::Shot;

		//タイマーを初期化しておく
		attackTimer = 0;
	}
}

void GalaxyCannon::AttackTypeRapidFireShot()
{
	//発射にかかる時間
	const float shotModeTime = 5;
	//タイマー更新
	attackTimer++;
	const float time = attackTimer / shotModeTime;

	//イージングで一気に押し出す
	const float easeStart = basePos.x - 0.3f;
	const float easeEnd = basePos.x + 0.25f;
	position.x = Easing::OutQuint(easeStart, easeEnd, time);

	//発射するタイミングになったら発射
	const int shotTime = 3;
	if (attackTimer == shotTime) {
		//弾発射
		const float bulletSize = 3.0f;
		const float bulletSpeed = 0.7f;
		Fire(bulletSize, bulletSpeed);

		//発射回数更新
		rapidFireCount++;
	}

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= shotModeTime) {
		attackTypeRapidFirePhase = AttackTypeRapidFirePhase::Return;

		//タイマーを初期化しておく
		attackTimer = 0;
	}
}

void GalaxyCannon::AttackTypeRapidFireReturn()
{
	//戻すのにかかる時間
	const float returnTime = 10;
	//タイマー更新
	attackTimer++;
	const float time = attackTimer / returnTime;

	//イージングで戻す
	const float easeStart = basePos.x + 0.25f;
	const float easeEnd = basePos.x;
	position.x = Easing::InQuad(easeStart, easeEnd, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= returnTime) {
		attackTypeRapidFirePhase = AttackTypeRapidFirePhase::Wait;

		//タイマーを初期化しておく
		attackTimer = 0;
	}
}

void GalaxyCannon::AttackTypeRapidFireWait()
{
	//攻撃中でなければ抜ける
	if (!isAttack) { return; }

	//既に指定した回数発射していたら攻撃終了させて抜ける
	const int rapidFireNum = 10;
	if (rapidFireCount >= rapidFireNum) {
		//攻撃状態終了
		isAttack = false;

		return;
	}

	//待機時間
	const float waitTime = 20;
	//タイマー更新
	attackTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= waitTime) {
		attackTypeRapidFirePhase = AttackTypeRapidFirePhase::Charge;

		//タイマーを初期化しておく
		attackTimer = 0;
	}
}

void GalaxyCannon::Break()
{
	//破壊用エフェクト追加
	for (int i = 0; i < breakModels.size(); i++) {
		//モデルが未設定なら飛ばす
		if (!breakModels[i]) { continue; }

		//ランダムでエフェクトの速度をセット
		const Vector3 randVel = { 4, 4, 4 };
		Vector3 velocity;
		velocity.x = (float)((rand() % (int)randVel.x) - randVel.x / 2);
		velocity.y = (float)((rand() % (int)randVel.y) - randVel.y / 2);
		velocity.z = -(float)((rand() % (int)randVel.z));

		//ランダムでエフェクトの回転の速さをセット
		const Vector3 randRotSpeed = { 5, 5, 5 };
		Vector3 rotSpeed;
		rotSpeed.x = (float)((rand() % (int)randRotSpeed.x) - randRotSpeed.x / 2);
		rotSpeed.y = (float)((rand() % (int)randRotSpeed.y) - randRotSpeed.y / 2);
		rotSpeed.z = (float)((rand() % (int)randRotSpeed.z) - randRotSpeed.z / 2);

		//値が大きいので割り算して小さくする
		const float div = 2;
		velocity /= div;

		//大きさをセット
		const Vector3 size = scale * 20;

		//破壊用エフェクトを生成
		std::unique_ptr<EnemyBreakEffect> newBreakEffect;
		newBreakEffect.reset(EnemyBreakEffect::Create(breakModels[i], bulletShotPos, velocity, rotSpeed, size));
		stageScene->AddEnemyBreakEffect(std::move(newBreakEffect));
	}
}
