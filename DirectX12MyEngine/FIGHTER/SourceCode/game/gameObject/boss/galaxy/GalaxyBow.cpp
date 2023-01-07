#include "GalaxyBow.h"
#include "Easing.h"
#include "BaseStageScene.h"
#include "BossChargeBullet.h"
#include "ParticleEmitter.h"

BaseStageScene* GalaxyBow::stageScene = nullptr;
ObjModel* GalaxyBow::bowModel = nullptr;
ObjModel* GalaxyBow::bowDeadModel = nullptr;
ObjModel* GalaxyBow::bulletModel = nullptr;
const Vector3 GalaxyBow::normalSize = { 0.3f, 0.3f, 0.3f };
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
	assert(bowModel);
	galaxyBow->model = bowModel;

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
	const Vector3 distancePos = { 0, 0, scale.z * parent->GetScale().z - 1.0f };

	//弾発射座標を取得
	bulletShotPos = LocalTranslation(distancePos, matWorld);
}

void GalaxyBow::AttackModeStart()
{
	//攻撃中にする
	isAttack = true;
}

void GalaxyBow::Fire(const float scale, const float bulletSpeed)
{
	//弾の速度を設定
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, bulletShotPos, velocity, scale));
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
