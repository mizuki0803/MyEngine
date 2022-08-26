#include "BossAvatarType01.h"
#include "Easing.h"

BossAvatarType01* BossAvatarType01::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//ボス(分身：タイプ01)のインスタンスを生成
	BossAvatarType01* bossAvatarType01 = new BossAvatarType01();
	if (bossAvatarType01 == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	bossAvatarType01->model = model;

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

	//親子関係上での定位置をセット
	bossAvatarType01->fixedPos = position;

	return bossAvatarType01;
}

void BossAvatarType01::AttackTypeBLockon()
{
	//タイマーを更新
	const float backTime = 120;
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

void BossAvatarType01::AttackTypeCMoveCenter()
{
	//タイマーを更新
	const float moveTime = 120;
	attackCTimer++;
	const float time = attackCTimer / moveTime;

	//中心に移動させる
	position.x = Easing::OutQuad(fixedPos.x, attackCLength, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackCTimer >= moveTime) {
		attackCPhase = AttackTypeCPhase::RotStart;

		//回転開始用に角度を設定する
		attackCAngle = 0;

		//タイマー初期化
		attackCTimer = 0;
	}
}
