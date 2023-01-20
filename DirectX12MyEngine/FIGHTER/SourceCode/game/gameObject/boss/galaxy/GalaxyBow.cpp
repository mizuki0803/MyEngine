#include "GalaxyBow.h"
#include "Easing.h"
#include "BaseStageScene.h"
#include "BossChargeBullet.h"
#include "EnemyFireBullet.h"
#include "ParticleEmitter.h"

void (GalaxyBow::* GalaxyBow::attackTypeFlamethrowerPhaseFuncTable[])() = {
	&GalaxyBow::AttackTypeFlamethrowerCharge,
	&GalaxyBow::AttackTypeFlamethrowerShot ,
	&GalaxyBow::AttackTypeFlamethrowerWait,
};

BaseStageScene* GalaxyBow::stageScene = nullptr;
ObjModel* GalaxyBow::bowModel = nullptr;
ObjModel* GalaxyBow::bowSleepModel = nullptr;
ObjModel* GalaxyBow::bowDeadModel = nullptr;
ObjModel* GalaxyBow::bulletModel = nullptr;
const Vector3 GalaxyBow::normalSize = { 0.25f, 0.25f, 0.25f };
const Vector3 GalaxyBow::damageSize = normalSize * 1.1f;
const XMFLOAT4 GalaxyBow::damageColor = { 1, 0.2f, 0.2f, 1 };


GalaxyBow* GalaxyBow::Create(ObjObject3d* parent, const Vector3& position)
{
	//ギャラクシー(船首)のインスタンスを生成
	GalaxyBow* galaxyBow = new GalaxyBow();
	if (galaxyBow == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(bowSleepModel);
	galaxyBow->model = bowSleepModel;

	// 初期化
	if (!galaxyBow->Initialize()) {
		delete galaxyBow;
		assert(0);
		return nullptr;
	}

	//親をセット
	galaxyBow->parent = parent;

	//座標をセット
	galaxyBow->position = position;

	//大きさをセット
	galaxyBow->scale = normalSize;

	return galaxyBow;
}

void GalaxyBow::Update()
{
	//ダメージ状態の処理
	if (isDamage) {
		DamageMode();
	}

	//オブジェクト更新
	ObjObject3d::Update();
}

void GalaxyBow::Damage(int attackPower, const Vector3& collisionPos)
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
		model = bowDeadModel;

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

void GalaxyBow::UpdateBulletShotPos()
{
	//中心座標からの距離
	const Vector3 distancePos = { 0, 0, -(scale.z * parent->GetScale().z) / 4 };

	//弾発射座標を取得
	bulletShotPos = Vector3::LocalTranslation(distancePos, matWorld);
}

void GalaxyBow::AttackModeStart()
{
	//攻撃内容:速射の変数の初期化
	attackTypeFlamethrowerPhase = AttackTypeFlamethrowerPhase::Charge;

	//攻撃で使うタイマーを初期化
	attackTimer = 0;

	//攻撃中にする
	isAttack = true;
}

void GalaxyBow::AttackTypeFlamethrower(const Vector3& playerPos)
{
	//標的の座標を更新
	fireTargetPosition = playerPos;

	//攻撃内容:火炎放射行動
	(this->*attackTypeFlamethrowerPhaseFuncTable[static_cast<size_t>(attackTypeFlamethrowerPhase)])();
}

void GalaxyBow::ChangeModel()
{
	//起きている状態のモデルをセット
	model = bowModel;
}

void GalaxyBow::Fire(const float scale, const float bulletSpeed)
{
	//弾の速度を設定
	Vector3 velocity(0, 0, -bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, bulletShotPos, velocity, scale));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void GalaxyBow::RockonFire(const float scale, const float bulletSpeed)
{
	//座標が標的より手前なら発射しない
	if (GetWorldPos().z <= fireTargetPosition.z) { return; }

	//弾の速度を設定
	Vector3 DifferenceVec = fireTargetPosition - bulletShotPos;
	DifferenceVec.normalize();
	Vector3 velocity = DifferenceVec * bulletSpeed;

	//弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyFireBullet::Create(bulletShotPos, velocity, scale));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void GalaxyBow::DamageMode()
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

void GalaxyBow::DamageSizeReturn()
{
	//大きくしたサイズを元に戻す時間
	const float sizeReturnTime = 10;
	//指定した時間以上なら抜ける
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//大きさを元に戻す
	scale = Easing::LerpVec3(damageSize, normalSize, time);
}

void GalaxyBow::DamageExplosion(const Vector3& collisionPos)
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

void GalaxyBow::DamageColorChange()
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

void GalaxyBow::AttackTypeFlamethrowerCharge()
{
	//チャージにかかる時間
	const float chargeTime = 180;
	//タイマー更新
	attackTimer++;

	//爆発させる間隔の時間
	const int explosionInterval = 3;
	if (attackTimer % explosionInterval == 0) {
		//爆発
		//船首の口元に合わせる
		Vector3 pos = bulletShotPos;
		//ランダムでずらす
		const Vector3 randPos = { 2, 2, 1 };
		pos.x += (float)((rand() % (int)randPos.x) - randPos.x / 2);
		pos.y += (float)((rand() % (int)randPos.y) - randPos.y / 2);
		pos.z += (float)((rand() % (int)randPos.z));

		//炎演出用パーティクル生成
		const float size = 0.5f;
		ParticleEmitter::GetInstance()->Explosion(pos, size);
	}

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= chargeTime) {
		attackTypeFlamethrowerPhase = AttackTypeFlamethrowerPhase::Shot;

		//タイマーを初期化しておく
		attackTimer = 0;
	}
}

void GalaxyBow::AttackTypeFlamethrowerShot()
{
	//発射する時間
	const float shotModeTime = 450;
	//タイマー更新
	attackTimer++;

	//発射する間隔になったら発射
	const int shotInterval = 10;
	if (attackTimer % shotInterval == 0) {
		//弾発射
		const float bulletSize = 2.0f;
		const float bulletSpeed = 1.0f;
		RockonFire(bulletSize, bulletSpeed);
	}

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= shotModeTime) {
		attackTypeFlamethrowerPhase = AttackTypeFlamethrowerPhase::Wait;

		//攻撃状態終了
		isAttack = false;

		//タイマーを初期化しておく
		attackTimer = 0;
	}
}

void GalaxyBow::AttackTypeFlamethrowerWait()
{
}
