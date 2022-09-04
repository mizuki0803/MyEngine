#include "BossAvatarType03.h"
#include "Easing.h"

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
