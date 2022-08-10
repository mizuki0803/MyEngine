#include "BossBody.h"

BossBody* BossBody::Create(ObjModel* model, const Vector3& position)
{
	//ボス(体)のインスタンスを生成
	BossBody* bossBody = new BossBody();
	if (bossBody == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	bossBody->model = model;

	// 初期化
	if (!bossBody->Initialize()) {
		delete bossBody;
		assert(0);
		return nullptr;
	}

	//座標をセット
	bossBody->position = position;

	return bossBody;
}
