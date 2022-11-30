#include "BossAvatar.h"
#include "Easing.h"
#include "GameScene.h"
#include "BossChargeBullet.h"
#include "ParticleEmitter.h"

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
ObjModel* BossAvatar::avatarDamageModel = nullptr;
ObjModel* BossAvatar::avatarSleepModel = nullptr;
ObjModel* BossAvatar::bulletModel = nullptr;
const Vector3 BossAvatar::normalSize = { 0.75f, 0.75f, 0.75f };
const Vector3 BossAvatar::damageSize = normalSize * 1.1f;
const float BossAvatar::attackModeRotY = 180.0f;
const float BossAvatar::waitModeRotY = 0.0f;
const float BossAvatar::attackAvatarGatlingLength = 1.25f;
const XMFLOAT4 BossAvatar::damageColor = { 1, 0.2f, 0.2f, 1 };

bool BossAvatar::Initialize()
{
	//大きさは本体の3/4
	scale = normalSize;

	//オブジェクト初期化
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	return true;
}

void BossAvatar::Update()
{
	//ダメージ状態のみの処理
	if (isDamage) {
		DamageMode();
	}

	//死亡時の動き
	if (isDead) {
		DeadAction();
	}

	//オブジェクト更新
	ObjObject3d::Update();
}

void BossAvatar::Damage(int attackPower, const Vector3& collisionPos, const Vector3& subjectVel)
{
	//引数の攻撃力をダメージ量にセット
	damageNum = attackPower;

	//ダメージを与える
	HP -= damageNum;

	//HPが0以下になったら死亡
	if (HP <= 0) {
		Dead();

		//HPゲージバグを起こさないようマイナス分を0に調整
		damageNum += HP;

		//爆発演出用パーティクル生成
		const float size = 3.0f;
		ParticleEmitter::GetInstance()->Explosion(GetWorldPos(), size);
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

void BossAvatar::DamageModelChange()
{
	//既にHPが少ない状態のモデルがセットされていたら抜ける
	if (isDamageModel) { return; }

	//HPが指定した値以下でないなら抜ける
	const int modelChangeHP = maxHP / 2;
	if (!(HP <= modelChangeHP)) { return; }

	//HPが少ない状態のモデルをセットする
	model = avatarDamageModel;
	//HPが少ない状態のモデルがセットされている状態にする
	isDamageModel = true;
}

void BossAvatar::UpdateBulletShotPos()
{
	//中心座標からの距離
	const Vector3 distancePos = { 0, 0, scale.z * parent->GetScale().z - 1.0f };
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

void BossAvatar::SetReturnBasePosition()
{
	//呼び出した瞬間の座標を基準位置に戻るときの出発座標として記録しておく
	returnStartPos = position;
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
	newBullet.reset(EnemyBullet::Create(bulletModel, bulletShotPos, velocity, scale));
	gameScene->AddEnemyBullet(std::move(newBullet));
}

void BossAvatar::ChargeBulletFire(const float scale, const float bulletSpeed)
{
	//弾の速度を設定
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//チャージ弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(BossChargeBullet::Create(bulletShotPos, velocity, scale));
	gameScene->AddEnemyBullet(std::move(newBullet));
}

void BossAvatar::DamageMode()
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

void BossAvatar::SetDamageKnockback(const Vector3& subjectVel)
{
	//ノックバックする方向を決める(対象の速度の方向)
	knockbackVec = subjectVel;
	knockbackVec.normalize();
}

void BossAvatar::DamageKnockback()
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

void BossAvatar::DamageSizeReturn()
{
	//大きくしたサイズを元に戻す時間
	const float sizeReturnTime = 10;
	//指定した時間以上なら抜ける
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//大きさを元に戻す
	scale = Easing::LerpVec3(damageSize, normalSize, time);

	//親がいないときはボス本体の大きさを持ってきてスケールをかける
	if (!parent) { scale *= BossMainBody::GetNormalSize().z; }
}

void BossAvatar::DamageExplosion(const Vector3& collisionPos)
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

	//ショット死亡演出用パーティクル生成
	const float size = 0.75f;
	ParticleEmitter::GetInstance()->Explosion(pos, size);
}

void BossAvatar::DamageColorMode()
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

	//発射する弾の大きさ
	const float bulletScale = 8.0f;
	//チャージ演出
	const float effectScale = Easing::OutCubic(0, bulletScale, time);
	ParticleEmitter::GetInstance()->BossCharge(bulletShotPos, effectScale);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= chargeTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Recoil;

		//タイマー初期化
		attackTimer = 0;

		//弾を発射
		const float bulletSpeed = 0.8f;
		ChargeBulletFire(bulletScale, bulletSpeed);

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

void BossAvatar::Dead()
{
	//死亡状態にする
	isDead = true;

	//死亡しているのに親に合わせて回転などをしてしまうと違和感があるので、親子構造を解除する
	//親子構造解除に伴い、座標と大きさを設定し直す
	position = GetWorldPos();
	scale = { matWorld.r[0].m128_f32[0], matWorld.r[1].m128_f32[1], matWorld.r[2].m128_f32[2] };
	parent = nullptr;
}

void BossAvatar::DeadAction()
{
	//X軸回転をしながら墜落する
	const float rotSpeed = 1.0f;
	rotation.x += rotSpeed;

	//墜落するため、速度に加速度を加える
	Vector3 crashAccel = { 0, -0.005f, 0 };
	crashVel += crashAccel;
	//落下する速度の最大値を設定
	const float maxCrashSpeed = -0.4f;
	if (crashVel.y <= maxCrashSpeed) { crashVel.y = maxCrashSpeed; }
	position += crashVel;

	//Y座標が0以下になったら削除
	if (GetWorldPos().y <= 0) {
		isDelete = true;

		//爆発演出用パーティクル生成
		ParticleEmitter::GetInstance()->Explosion(GetWorldPos());
	}
}
