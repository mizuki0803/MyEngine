#include "BossAvatarType04.h"
#include "Easing.h"

BossAvatarType04* BossAvatarType04::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//ボス(分身：タイプ04)のインスタンスを生成
	BossAvatarType04* bossAvatarType04 = new BossAvatarType04();
	if (bossAvatarType04 == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	bossAvatarType04->model = model;

	// 初期化
	if (!bossAvatarType04->Initialize()) {
		delete bossAvatarType04;
		assert(0);
		return nullptr;
	}

	//親をセット
	bossAvatarType04->parent = parent;

	//座標をセット
	bossAvatarType04->position = position;

	//親子関係上での定位置をセット
	bossAvatarType04->fixedPos = position;

	return bossAvatarType04;
}

void BossAvatarType04::AttackTypeBLockon()
{
	//タイマーを更新
	const float backTime = 180;
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

void BossAvatarType04::AttackTypeCMoveCenter()
{
	//タイマーを更新
	const float moveTime = 120;
	attackCTimer++;
	const float time = attackCTimer / moveTime;

	//中心に移動させる
	position.y = Easing::OutQuad(fixedPos.y, -attackCLength, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackCTimer >= moveTime) {
		attackCPhase = AttackTypeCPhase::RotStart;

		//回転開始用に角度を設定する
		attackCAngle = 270;

		//タイマー初期化
		attackCTimer = 0;
	}
}

void BossAvatarType04::AttackTypeDWait()
{
	//タイマーを更新
	const float waitTime = 60;
	attackDTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackDTimer >= waitTime) {
		attackDPhase = AttackTypeDPhase::Move;

		//タイマー初期化
		attackDTimer = 0;
	}
}

void BossAvatarType04::AttackTypeDMove()
{
	//タイマーを更新
	const float moveTime = 150;
	attackDTimer++;
	const float time = attackDTimer / moveTime;

	//発射位置に移動させる
	const Vector3 movePos = { 7, 0, 0 };
	position.x = Easing::OutQuad(fixedPos.x, movePos.x, time);
	position.y = Easing::OutQuad(fixedPos.y, movePos.y, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackDTimer >= moveTime) {
		attackDPhase = AttackTypeDPhase::ChargeShot;

		//タイマー初期化
		attackDTimer = 0;
	}
}
