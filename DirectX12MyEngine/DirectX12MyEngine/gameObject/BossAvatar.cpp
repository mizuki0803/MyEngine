#include "BossAvatar.h"
#include "Easing.h"
#include "GameScene.h"

void (BossAvatar::* BossAvatar::attackTypeAvatarBodyBlowPhaseFuncTable[])() = {
	&BossAvatar::AttackTypeAvatarBodyBlowLockon,
	&BossAvatar::AttackTypeAvatarBodyBlowShot,
	&BossAvatar::AttackTypeAvatarBodyBlowBack,
	&BossAvatar::Stay,
};

void (BossAvatar::* BossAvatar::attackTypeAvatarGatlingPhaseFuncTable[])() = {
	&BossAvatar::AttackTypeAvatarGatlingMoveCenter,
	&BossAvatar::AttackTypeAvatarGatlingRotStart,
	&BossAvatar::AttackTypeAvatarGatlingShot,
	&BossAvatar::AttackTypeAvatarGatlingRotEnd,
	&BossAvatar::Stay,
};

void (BossAvatar::* BossAvatar::attackTypeAvatarGiantBulletPhaseFuncTable[])() = {
	&BossAvatar::AttackTypeAvatarGiantBulletWait,
	&BossAvatar::AttackTypeAvatarGiantBulletMove,
	&BossAvatar::AttackTypeAvatarGiantBulletChargeShot,
	&BossAvatar::AttackTypeAvatarGiantBulletRecoil,
	&BossAvatar::Stay,
};


GameScene* BossAvatar::gameScene = nullptr;
ObjModel* BossAvatar::avatarModel = nullptr;
ObjModel* BossAvatar::avatarSleepModel = nullptr;
ObjModel* BossAvatar::bulletModel = nullptr;
const float BossAvatar::attackModeRotY = 180.0f;
const float BossAvatar::waitModeRotY = 0.0f;
const float BossAvatar::attackAvatarGatlingLength = 1.25f;

bool BossAvatar::Initialize()
{
	//大きさは本体の3/4
	scale = { 0.75f, 0.75f, 0.75f };

	if (!ObjObject3d::Initialize()) {
		return false;
	}

	return true;
}

void BossAvatar::Update()
{
	//ダメージ色状態のみの処理
	if (isDamageColor) {
		DamageColorMode();
	}

	//オブジェクト更新
	ObjObject3d::Update();
}

void BossAvatar::Damage(int attackPower)
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

void BossAvatar::AttackTypeAvatarBodyBlow(const Vector3& playerPosition)
{
	//自機座標をロックオンする
	attackAvatarBodyBlowLockonPos = playerPosition;

	//行動
	(this->*attackTypeAvatarBodyBlowPhaseFuncTable[static_cast<size_t>(attackAvatarBodyBlowPhase)])();
}

void BossAvatar::AttackTypeAvatarGatling()
{
	//行動
	(this->*attackTypeAvatarGatlingPhaseFuncTable[static_cast<size_t>(attackAvatarGatlingPhase)])();
}

void BossAvatar::AttackTypeAvatarGiantBullet()
{
	//行動
	(this->*attackTypeAvatarGiantBulletPhaseFuncTable[static_cast<size_t>(attackAvatarGiantBulletPhase)])();
}

void BossAvatar::ChangeAttackMode(const float time)
{
	//180度回転させて反対向きにする
	rotation.y = Easing::InOutBack(waitModeRotY, attackModeRotY, time);
}

void BossAvatar::ChangeWaitMode(const float time)
{
	//180度回転させて反対向きにする
	rotation.x = 0;
	rotation.y = Easing::InOutBack(attackModeRotY, waitModeRotY, time);
	rotation.z = 0;
}

void BossAvatar::ReturnBasePosition(const float time)
{
	//親子関係上での基準位置座標に戻す
	position.x = Easing::OutQuad(returnStartPos.x, basePos.x, time);
	position.y = Easing::OutQuad(returnStartPos.y, basePos.y, time);
	position.z = Easing::OutQuad(returnStartPos.z, basePos.z, time);
}

void BossAvatar::AttackEnd()
{
	//呼び出した瞬間の座標を基準位置に戻るときの出発座標として記録しておく
	returnStartPos = position;

	//弾発射タイマーを初期化
	fireTimer = 0;

	//攻撃タイマーを初期化
	attackTimer = 0;

	//攻撃内容:分身体当たりの変数の初期化
	attackAvatarBodyBlowPhase = AttackTypeAvatarBodyBlowPhase::Lockon;

	//攻撃内容:分身ガトリング砲の変数の初期化
	attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::MoveCenter;
	attackAvatarGatlingRotSpeed = 0;

	//攻撃内容:分身巨大弾の変数の初期化
	attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Wait;
	attackAvatarGiantBulletRecoilVelocity = { 0, 0, 2 };
	attackAvatarGiantBulletRecoilAccel = { 0, 0, -0.05f };
}

void BossAvatar::ChangeModel()
{
	//起きている状態のモデルをセット
	model = avatarModel;
}

Vector3 BossAvatar::GetWorldPos()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//平行移動成分を取得
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void BossAvatar::Fire(const float scale, const float bulletSpeed)
{
	//弾の速度を設定
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, GetWorldPos(), velocity, scale));
	gameScene->AddEnemyBullet(std::move(newBullet));
}

void BossAvatar::DamageColorMode()
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

void BossAvatar::AttackTypeAvatarBodyBlowShot()
{
	//ロックオンで決めた速度で飛ばす
	position += attackAvatarBodyBlowVelocity;

	//タイマーを更新
	const float shotTime = 180;
	attackTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= shotTime) {
		attackAvatarBodyBlowPhase = AttackTypeAvatarBodyBlowPhase::Back;

		//タイマー初期化
		attackTimer = 0;
	}
}

void BossAvatar::AttackTypeAvatarBodyBlowBack()
{
	//タイマーを更新
	const float backTime = 120;
	attackTimer++;
	const float time = attackTimer / backTime;

	//親子関係上での基準座標に戻す
	position.x = Easing::OutQuad(0, basePos.x, time);
	position.y = Easing::OutQuad(60, basePos.y, time);
	position.z = Easing::OutQuad(0, basePos.z, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= backTime) {
		attackAvatarBodyBlowPhase = AttackTypeAvatarBodyBlowPhase::Stay;

		//タイマー初期化
		attackTimer = 0;
	}
}

void BossAvatar::AttackTypeAvatarGatlingRotStart()
{
	//回転を開始(回転速度を速くする)
	const float rotAccelSpeed = 0.1f;
	attackAvatarGatlingRotSpeed += rotAccelSpeed;

	//角度を弧度法に変換
	float radius = XMConvertToRadians(attackAvatarGatlingAngle);

	//円の中心から指定した半径の長さをぐるぐる回る
	position.x = cos(radius) * attackAvatarGatlingLength;
	position.y = sin(radius) * attackAvatarGatlingLength;

	//角度の更新
	attackAvatarGatlingAngle += attackAvatarGatlingRotSpeed;

	//回転速度が指定した速度以上になったら次のフェーズへ
	const float maxRotSpeed = 4.0f;
	if (attackAvatarGatlingRotSpeed >= maxRotSpeed) {
		attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::Shot;
	}
}

void BossAvatar::AttackTypeAvatarGatlingShot()
{
	//タイマーを更新
	const float moveTime = 300;
	attackTimer++;

	//角度を弧度法に変換
	float radius = XMConvertToRadians(attackAvatarGatlingAngle);

	//円の中心から指定した半径の長さをぐるぐる回る
	position.x = cos(radius) * attackAvatarGatlingLength;
	position.y = sin(radius) * attackAvatarGatlingLength;

	//角度の更新
	attackAvatarGatlingAngle += attackAvatarGatlingRotSpeed;

	//弾発射タイマーカウント
	fireTimer++;
	const int fireInterval = 10;
	if (fireTimer >= fireInterval) {
		//弾を発射
		const float bulletScale = 1.0f;
		const float bulletSpeed = 0.75f;
		Fire(bulletScale, bulletSpeed);
		//発射タイマーを初期化
		fireTimer = 0;
	}

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= moveTime) {
		attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::RotEnd;

		//タイマー初期化
		attackTimer = 0;
	}
}

void BossAvatar::AttackTypeAvatarGatlingRotEnd()
{
	//回転を終了(回転速度を遅くする)
	const float rotAccelSpeed = -0.1f;
	attackAvatarGatlingRotSpeed += rotAccelSpeed;

	//角度を弧度法に変換
	float radius = XMConvertToRadians(attackAvatarGatlingAngle);

	//円の中心から指定した半径の長さをぐるぐる回る
	position.x = cos(radius) * attackAvatarGatlingLength;
	position.y = sin(radius) * attackAvatarGatlingLength;

	//角度の更新
	attackAvatarGatlingAngle += attackAvatarGatlingRotSpeed;

	//回転速度が0以下になったら次のフェーズへ
	if (attackAvatarGatlingRotSpeed <= 0) {
		attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::Stay;
	}
}

void BossAvatar::AttackTypeAvatarGiantBulletChargeShot()
{
	//タイマーを更新
	const float chargeTime = 180;
	attackTimer++;
	const float time = attackTimer / chargeTime;

	//色を赤くする
	color.y = Easing::OutQuad(1, 0, time);
	color.z = Easing::OutQuad(1, 0, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= chargeTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Recoil;

		//タイマー初期化
		attackTimer = 0;

		//弾を発射
		const float bulletScale = 10.0f;
		const float bulletSpeed = 0.8f;
		Fire(bulletScale, bulletSpeed);

		//色を元に戻す
		color = { 1, 1, 1, 1 };
	}
}

void BossAvatar::AttackTypeAvatarGiantBulletRecoil()
{
	//反動で奥に移動させる
	position += attackAvatarGiantBulletRecoilVelocity;
	attackAvatarGiantBulletRecoilVelocity += attackAvatarGiantBulletRecoilAccel;

	//反動の速度が0になったら次のフェーズへ
	if (attackAvatarGiantBulletRecoilVelocity.z <= 0) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Stay;
	}
}

void BossAvatar::Stay()
{
}
