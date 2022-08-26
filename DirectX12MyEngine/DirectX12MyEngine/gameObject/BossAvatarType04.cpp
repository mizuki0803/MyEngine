#include "BossAvatarType04.h"

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
