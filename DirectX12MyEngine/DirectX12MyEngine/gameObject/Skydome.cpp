#include "Skydome.h"

Skydome* Skydome::Create(ObjModel* model)
{
	//天球のインスタンスを生成
	Skydome* skydome = new Skydome();
	if (skydome == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!skydome->Initialize()) {
		delete skydome;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(model);
	skydome->model = model;
	skydome->scale = { 5, 5, 5 };

	return skydome;
}
