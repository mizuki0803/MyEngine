#include "BossAvatarType03.h"
#include "Easing.h"

BossAvatarType03* BossAvatarType03::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//ボス(分身：タイプ03)のインスタンスを生成
	BossAvatarType03* bossAvatarType03 = new BossAvatarType03();
	if (bossAvatarType03 == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	bossAvatarType03->model = model;

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

void BossAvatarType03::AttackTypeBLockon()
{
	//タイマーを更新
	const float backTime = 210;
	attackBTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackBTimer >= backTime) {
		attackBPhase = AttackTypeBPhase::Shot;

		//飛ばす速度を確定させる
		attackBVelocity = attackBLockonPos - GetWorldPos();
		const float moveSpeed = 1.0f;
		attackBVelocity = attackBVelocity.normalize() * moveSpeed;

		//タイマー初期化
		attackBTimer = 0;
	}
}

void BossAvatarType03::AttackTypeCMoveCenter()
{
	//タイマーを更新
	const float moveTime = 120;
	attackCTimer++;
	const float time = attackCTimer / moveTime;

	//親子関係上での基準位置に移動させる
	position.y = Easing::OutQuad(basePos.y, attackCLength, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackCTimer >= moveTime) {
		attackCPhase = AttackTypeCPhase::RotStart;

		//回転開始用に角度を設定する
		attackCAngle = 90;

		//タイマー初期化
		attackCTimer = 0;
	}
}

void BossAvatarType03::AttackTypeDWait()
{
	//タイマーを更新
	const float waitTime = 30;
	attackDTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackDTimer >= waitTime) {
		attackDPhase = AttackTypeDPhase::Move;

		//タイマー初期化
		attackDTimer = 0;
	}
}

void BossAvatarType03::AttackTypeDMove()
{
	//タイマーを更新
	const float moveTime = 150;
	attackDTimer++;
	const float time = attackDTimer / moveTime;

	//発射位置に移動させる
	const Vector3 movePos = { -7, 0, 0 };
	position.x = Easing::OutQuad(basePos.x, movePos.x, time);
	position.y = Easing::OutQuad(basePos.y, movePos.y, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackDTimer >= moveTime) {
		attackDPhase = AttackTypeDPhase::ChargeShot;

		//タイマー初期化
		attackDTimer = 0;
	}
}
