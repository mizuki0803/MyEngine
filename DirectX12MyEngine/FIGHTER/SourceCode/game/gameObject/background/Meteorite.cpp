#include "Meteorite.h"

ObjModel* Meteorite::meteoriteModel = nullptr;
GameCamera* Meteorite::gameCamera = nullptr;

Meteorite* Meteorite::Create(const Vector3& position, const Vector3& rotation, float size, const Vector3& velocity, const Vector3& rotSpeed)
{
	//隕石のインスタンスを生成
	Meteorite* meteorite = new Meteorite();
	if (meteorite == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!meteorite->Initialize()) {
		delete meteorite;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(meteoriteModel);
	meteorite->model = meteoriteModel;
	//座標をセット
	meteorite->position = position;
	//回転速度をセット
	meteorite->rotSpeed = rotSpeed;
	//大きさをセット
	meteorite->scale = { size, size, size };
	//移動速度をセット
	meteorite->velocity = velocity;
	//回転角をセット
	meteorite->rotation = rotation;

	return meteorite;
}

void Meteorite::Update()
{
	//座標を更新
	position += velocity;
	//回転角を更新
	rotation += rotSpeed;

	//オブジェクト更新
	ObjObject3d::Update();

	//手前まで行ったら削除
	FrontOfScreenDelete();
}

void Meteorite::FrontOfScreenDelete()
{
	//座標が自機より手前(画面外手前)まで行ったら削除
	const float flontOfScreenDiffence = 50;
	const float deletePos = gameCamera->GetPosition().z - flontOfScreenDiffence;

	if (position.z <= deletePos) {
		isDelete = true;
	}
}
