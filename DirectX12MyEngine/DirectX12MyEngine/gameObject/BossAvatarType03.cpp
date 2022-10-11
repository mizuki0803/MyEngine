#include "BossAvatarType03.h"
#include "Easing.h"
#include "ParticleEmitter.h"

BossAvatarType03* BossAvatarType03::Create(ObjObject3d* parent, const Vector3& position)
{
	//ボス(分身：タイプ03)のインスタンスを生成
	BossAvatarType03* bossAvatarType03 = new BossAvatarType03();
	if (bossAvatarType03 == nullptr) {
		return nullptr;
	}

	//寝ている状態のモデルをセット
	assert(avatarSleepModel);
	bossAvatarType03->model = avatarSleepModel;

	// 初期化
	if (!bossAvatarType03->Initialize()) {
		delete bossAvatarType03;
		assert(0);
		return nullptr;
	}

	//親をセット
	bossAvatarType03->parent = parent;

	//座標をセット
	bossAvatarType03->position = position;

	//親子関係上での基準位置をセット
	bossAvatarType03->basePos = position;

	return bossAvatarType03;
}

bool BossAvatarType03::Initialize()
{
	//共通の死亡時の動きをするとボス本体に被さってしまうため、Z方向にも動かす
	crashVel.z = 1.0f;

	//ボス分身共通初期化
	if (!BossAvatar::Initialize()) {
		return false;
	}

	return true;
}

void BossAvatarType03::AttackTypeAvatarBodyBlowLockon()
{
	//タイマーを更新
	const float backTime = 210;
	attackTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= backTime) {
		attackAvatarBodyBlowPhase = AttackTypeAvatarBodyBlowPhase::Shot;

		//飛ばす速度を確定させる
		attackAvatarBodyBlowVelocity = attackAvatarBodyBlowLockonPos - GetWorldPos();
		const float moveSpeed = 0.5f;
		attackAvatarBodyBlowVelocity = attackAvatarBodyBlowVelocity.normalize() * moveSpeed;

		//タイマー初期化
		attackTimer = 0;
	}
}

void BossAvatarType03::AttackTypeAvatarGatlingMoveCenter()
{
	//タイマーを更新
	const float moveTime = 120;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//親子関係上での基準位置に移動させる
	position.y = Easing::OutQuad(basePos.y, attackAvatarGatlingLength, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= moveTime) {
		attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::RotStart;

		//回転開始用に角度を設定する
		attackAvatarGatlingAngle = 90;

		//タイマー初期化
		attackTimer = 0;
	}
}

void BossAvatarType03::AttackTypeAvatarGiantBulletWait()
{
	//タイマーを更新
	const float waitTime = 30;
	attackTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= waitTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Move;

		//タイマー初期化
		attackTimer = 0;
	}
}

void BossAvatarType03::AttackTypeAvatarGiantBulletMove()
{
	//タイマーを更新
	const float moveTime = 150;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//発射位置に移動させる
	const Vector3 movePos = { -2, 0, 0 };
	position.x = Easing::OutQuad(basePos.x, movePos.x, time);
	position.y = Easing::OutQuad(basePos.y, movePos.y, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= moveTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::ChargeShot;

		//タイマー初期化
		attackTimer = 0;
	}
}

void BossAvatarType03::DeadAction()
{
	//X軸回転をしながら墜落する
	const float rotSpeed = 1.0f;
	rotation.x += rotSpeed;

	//共通の死亡時の動きをするとボス本体に被さってしまうため、Z方向にも動かす
	//墜落するため、速度に加速度を加える
	Vector3 crashAccel = { 0, -0.005f, -0.04f };
	crashVel += crashAccel;
	//落下する速度の最大値を設定
	const float maxCrashSpeed = -0.4f;
	if (crashVel.y <= maxCrashSpeed) { crashVel.y = maxCrashSpeed; }
	if (crashVel.z <= 0) { crashVel.z = 0; }
	position += crashVel;

	//Y座標が0以下になったら削除
	if (GetWorldPos().y <= 0) {
		isDelete = true;

		//爆発演出用パーティクル生成
		ParticleEmitter::GetInstance()->Explosion(GetWorldPos());
	}
}
