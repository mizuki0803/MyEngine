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

	return bossAvatarType02;
}
