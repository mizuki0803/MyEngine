#include "GalaxyPropeller.h"

ObjModel* GalaxyPropeller::propellerModel = nullptr;

GalaxyPropeller* GalaxyPropeller::Create(ObjObject3d* parent, const Vector3& position, const Vector3& rotation, const float rotSpeed, const float size)
{
	//ギャラクシー(プロペラ)のインスタンスを生成
	GalaxyPropeller* galaxyPropeller = new GalaxyPropeller();
	if (galaxyPropeller == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!galaxyPropeller->Initialize(parent, position, rotation, rotSpeed, size)) {
		delete galaxyPropeller;
		assert(0);
		return nullptr;
	}

	return galaxyPropeller;
}

bool GalaxyPropeller::Initialize(ObjObject3d* parent, const Vector3& position, const Vector3& rotation, const float rotSpeed, const float size)
{
	//オブジェクト初期化
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//モデルをセット
	assert(propellerModel);
	model = propellerModel;

	//親オブジェクトをセット
	this->parent = parent;
	//座標をセット
	this->position = position;
	//回転角をセット
	this->rotation = rotation;
	//回転速度をセット
	this->rotSpeed = rotSpeed;
	//大きさをセット
	scale = { size, size, size };

	return true;
}

void GalaxyPropeller::Update()
{
	//回転
	if (isRotation) {
		Rotation();
	}

	//オブジェクト更新
	ObjObject3d::Update();

	Vector3 a = GetWorldPos();
}

void GalaxyPropeller::Rotation()
{
	//回転速度を加算して回転させる
	rotation.y += rotSpeed;

	//角度修正
	if (rotation.y <= 0) { rotation.y += 360; }
	if (rotation.y >= 360) { rotation.y -= 360; }
}
