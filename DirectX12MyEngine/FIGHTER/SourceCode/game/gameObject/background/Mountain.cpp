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

void Mountain::FrontOfScreenDelete(const Vector3& pos)
{
	//座標が自機より手前(画面外手前)まで行ったら削除
	const float flontOfScreenDiffence = 50;
	const float deletePos = pos.z - flontOfScreenDiffence;

	if (position.z <= deletePos) {
		isDead = true;
	}
}
