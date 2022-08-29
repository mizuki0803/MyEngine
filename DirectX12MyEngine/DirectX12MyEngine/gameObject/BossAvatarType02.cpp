#include "BossAvatarType02.h"
#include "Easing.h"

BossAvatarType02* BossAvatarType02::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//ボス(分身：タイプ02)のインスタンスを生成
	BossAvatarType02* bossAvatarType02 = new BossAvatarType02();
	if (bossAvatarType02 == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	bossAvatarType02->model = model;

	// 初期化
	if (!bossAvatarType02->Initialize()) {
		delete bossAvatarType02;
		assert(0);
		return nullptr;
	}

	//親をセット
	bossAvatarType02->parent = parent;

	//座標をセット
	bossAvatarType02->position = position;

	//親子関係上での基準位置をセット
	bossAvatarType02->basePos = position;

	return bossAvatarType02;
}

void BossAvatarType02::AttackTypeBLockon()
{
	//タイマーを更新
	const float backTime = 150;
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

void BossAvatarType02::AttackTypeCMoveCenter()
{
	//タイマーを更新
	const float moveTime = 120;
	attackCTimer++;
	const float time = attackCTimer / moveTime;

	//親子関係上での基準位置に移動させる
	position.x = Easing::OutQuad(basePos.x, -attackCLength, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackCTimer >= moveTime) {
		attackCPhase = AttackTypeCPhase::RotStart;

		//回転開始用に角度を設定する
		attackCAngle = 180;

		//タイマー初期化
		attackCTimer = 0;
	}
}

void BossAvatarType02::AttackTypeDWait()
{
	//次のフェーズへ
	attackDPhase = AttackTypeDPhase::Move;
}

void BossAvatarType02::AttackTypeDMove()
{
	//タイマーを更新
	const float moveTime = 150;
	attackDTimer++;
	const float time = attackDTimer / moveTime;

	//発射位置に移動させる
	const Vector3 movePos = { -21, 0, 0 };
	position.x = Easing::OutQuad(basePos.x, movePos.x, time);
	position.y = Easing::OutQuad(basePos.y, movePos.y, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackDTimer >= moveTime) {
		attackDPhase = AttackTypeDPhase::ChargeShot;

		//タイマー初期化
		attackDTimer = 0;
	}
}
