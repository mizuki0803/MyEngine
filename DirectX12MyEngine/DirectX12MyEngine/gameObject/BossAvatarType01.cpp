#include "BossAvatarType01.h"
#include "Easing.h"

BossAvatarType01* BossAvatarType01::Create(ObjObject3d* parent, const Vector3& position)
{
	//ボス(分身：タイプ01)のインスタンスを生成
	BossAvatarType01* bossAvatarType01 = new BossAvatarType01();
	if (bossAvatarType01 == nullptr) {
		return nullptr;
	}

	//寝ている状態のモデルをセット
	assert(avatarSleepModel);
	bossAvatarType01->model = avatarSleepModel;

	// 初期化
	if (!bossAvatarType01->Initialize()) {
		delete bossAvatarType01;
		assert(0);
		return nullptr;
	}

	//親をセット
	bossAvatarType01->parent = parent;

	//座標をセット
	bossAvatarType01->position = position;

	//親子関係上での基準位置をセット
	bossAvatarType01->basePos = position;

	return bossAvatarType01;
}

void BossAvatarType01::AttackTypeAvatarBodyBlowLockon()
{
	//タイマーを更新
	const float backTime = 120;
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

void BossAvatarType01::AttackTypeAvatarGatlingMoveCenter()
{
	//タイマーを更新
	const float moveTime = 120;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//親子関係上での基準位置に移動させる
	position.x = Easing::OutQuad(basePos.x, attackAvatarGatlingLength, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= moveTime) {
		attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::RotStart;

		//回転開始用に角度を設定する
		attackAvatarGatlingAngle = 0;

		//タイマー初期化
		attackTimer = 0;
	}
}

void BossAvatarType01::AttackTypeAvatarGiantBulletWait()
{
	//タイマーを更新
	const float waitTime = 90;
	attackTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= waitTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Move;

		//タイマー初期化
		attackTimer = 0;
	}
}

void BossAvatarType01::AttackTypeAvatarGiantBulletMove()
{
	//タイマーを更新
	const float moveTime = 150;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//発射位置に移動させる
	const Vector3 movePos = { 6, 0, 0 };
	position.x = Easing::OutQuad(basePos.x, movePos.x, time);
	position.y = Easing::OutQuad(basePos.y, movePos.y, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= moveTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::ChargeShot;

		//タイマー初期化
		attackTimer = 0;
	}
}
