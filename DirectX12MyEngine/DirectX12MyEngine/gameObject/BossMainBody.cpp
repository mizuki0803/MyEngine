#include "BossMainBody.h"

BossMainBody* BossMainBody::Create(ObjModel* model, const Vector3& position)
{
	//ボス(本体)のインスタンスを生成
	BossMainBody* bossMainBody = new BossMainBody();
	if (bossMainBody == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	bossMainBody->model = model;

	// 初期化
	if (!bossMainBody->Initialize()) {
		delete bossMainBody;
		assert(0);
		return nullptr;
	}

	//座標をセット
	bossMainBody->position = position;

	return bossMainBody;
}
