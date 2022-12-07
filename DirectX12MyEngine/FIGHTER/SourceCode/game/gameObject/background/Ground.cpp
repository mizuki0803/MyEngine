#include "Ground.h"

Ground* Ground::Create(ObjModel* model)
{
	//地面のインスタンスを生成
	Ground* ground = new Ground();
	if (ground == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!ground->Initialize()) {
		delete ground;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(model);
	ground->model = model;

	//大きさをセット
	ground->scale = { 20, 20, 20 };

	//シャドウマップで影を付ける
	ground->isShadowMap = true;

	return ground;
}
