#include "Mountain.h"

Mountain* Mountain::Create(ObjModel* model, const Vector3& position)
{
	//山のインスタンスを生成
	Mountain* mountain = new Mountain();
	if (mountain == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!mountain->Initialize()) {
		delete mountain;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(model);
	mountain->model = model;

	//座標をセット
	mountain->position = position;

	//大きさをセット
	mountain->scale = { 10, 10, 10 };

	return mountain;
}
