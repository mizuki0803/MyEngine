#include "BossAvatar.h"

BossAvatar* BossAvatar::Create(ObjModel* model, ObjObject3d* parent)
{
	//ボス(分身)のインスタンスを生成
	BossAvatar* bossAvatar = new BossAvatar();
	if (bossAvatar == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	bossAvatar->model = model;

	// 初期化
	if (!bossAvatar->Initialize()) {
		delete bossAvatar;
		assert(0);
		return nullptr;
	}

	//親をセット
	bossAvatar->parent = parent;

	return bossAvatar;
}
