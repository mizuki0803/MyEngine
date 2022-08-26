#include "BossAvatarType02.h"

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

	//親子関係上での定位置をセット
	bossAvatarType02->fixedPos = position;

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
