#include "BossHead.h"

BossHead* BossHead::Create(ObjModel* model, ObjObject3d* parent)
{
	//ボス(頭)のインスタンスを生成
	BossHead* bossHead = new BossHead();
	if (bossHead == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	bossHead->model = model;

	// 初期化
	if (!bossHead->Initialize()) {
		delete bossHead;
		assert(0);
		return nullptr;
	}

	//親をセット
	bossHead->parent = parent;

	return bossHead;
}
