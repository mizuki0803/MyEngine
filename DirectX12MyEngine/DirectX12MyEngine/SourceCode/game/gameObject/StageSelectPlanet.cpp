#include "StageSelectPlanet.h"

StageSelectPlanet* StageSelectPlanet::Create(ObjModel* model, const Vector3& position)
{
	//ステージ選択用惑星のインスタンスを生成
	StageSelectPlanet* stageSelectPlanet = new StageSelectPlanet();
	if (stageSelectPlanet == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	stageSelectPlanet->model = model;

	// 初期化
	if (!stageSelectPlanet->Initialize()) {
		delete stageSelectPlanet;
		assert(0);
		return nullptr;
	}

	//初期座標をセット
	stageSelectPlanet->position = position;

	//大きさをセット
	const float size = 25.0f;
	stageSelectPlanet->scale = { size, size, size };

	return stageSelectPlanet;
}

void StageSelectPlanet::Update()
{
	//回転させる
	const float rotSpeed = 0.1f;
	rotation.y += rotSpeed;

	//オブジェクト更新
	ObjObject3d::Update();
}
