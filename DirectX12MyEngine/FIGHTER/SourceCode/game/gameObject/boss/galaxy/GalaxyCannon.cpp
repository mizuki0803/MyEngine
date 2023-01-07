#include "GalaxyCannon.h"
#include "Easing.h"
#include "BaseStageScene.h"
#include "BossChargeBullet.h"
#include "ParticleEmitter.h"

void (GalaxyCannon::* GalaxyCannon::attackTypeRapidFirePhaseFuncTable[])() = {
	&GalaxyCannon::AttackTypeRapidFireCharge,
	&GalaxyCannon::AttackTypeRapidFireShot,
	&GalaxyCannon::AttackTypeRapidFireReturn,
};

BaseStageScene* GalaxyCannon::stageScene = nullptr;
ObjModel* GalaxyCannon::cannonModel = nullptr;
ObjModel* GalaxyCannon::cannonDeadModel = nullptr;
ObjModel* GalaxyCannon::bulletModel = nullptr;
const Vector3 GalaxyCannon::normalSize = { 0.25f, 0.25f, 0.25f };
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

		//HPゲージバグを起こさないようマイナス分を0に調整
		damageNum += HP;

		//死亡用モデルに変更
		model = cannonDeadModel;

		//爆発演出用パーティクル生成
		const float size = 3.0f;
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

void GalaxyCannon::AttackModeStart()
{
	//攻撃内容:分身体当たりの変数の初期化
	attackTypeRapidFirePhase = AttackTypeRapidFirePhase::Charge;
	//攻撃で使うタイマーを初期化
	attackTimer = 0;

	//攻撃中にする
	isAttack = true;
}

void GalaxyCannon::AttackTypeRapidFireCharge()
{
	//チャージにかかる時間
	const float chargeTime = 60;
	//タイマー更新
	attackTimer++;
	const float time = attackTimer / chargeTime;

	//イージングで後ろに引っ張る
	const float easeStart = basePos.x;
	const float easeEnd = basePos.x - 0.5f;
	position.x = Easing::OutCubic(easeStart, easeEnd, time);

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
	const float shotModeTime = 15;
	//タイマー更新
	attackTimer++;
	const float time = attackTimer / shotModeTime;

	//イージングで一気に押し出す
	const float easeStart = basePos.x - 0.5f;
	const float easeEnd = basePos.x + 0.25f;
	position.x = Easing::OutQuint(easeStart, easeEnd, time);

	//発射するタイミングになったら発射
	const int shotTime = 10;
	if (attackTimer == shotTime) {
		//弾発射
		const float bulletSize = 1.5f;
		const float bulletSpeed = 2.0f;
		Fire(bulletSize, bulletSpeed);
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
	const float returnTime = 30;
	//タイマー更新
	attackTimer++;
	const float time = attackTimer / returnTime;

	//イージングで戻す
	const float easeStart = basePos.x + 0.25f;
	const float easeEnd = basePos.x;
	position.x = Easing::InQuad(easeStart, easeEnd, time);

	//タイマーが指定した時間になったら
	if (attackTimer >= returnTime) {
		//攻撃状態終了
		isAttack = false;

		//タイマーを初期化しておく
		attackTimer = 0;
	}
}
