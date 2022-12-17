#include "Building.h"

Building* Building::Create(ObjModel* model, const Vector3& position)
{
	//ビルのインスタンスを生成
	Building* building = new Building();
	if (building == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!building->Initialize()) {
		delete building;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(model);
	building->model = model;

	//座標をセット
	building->position = position;

	//大きさをセット
	building->scale = { 5, 5, 5 };

	return building;
}

void Building::FrontOfScreenDelete(const Vector3& pos)
{
	//座標が自機より手前(画面外手前)まで行ったら削除
	const float flontOfScreenDiffence = 80;
	const float deletePos = pos.z - flontOfScreenDiffence;

	if (position.z <= deletePos) {
		isDead = true;
	}
}
