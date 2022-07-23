#include "StraightBullet.h"

StraightBullet* StraightBullet::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
{
	//自機弾のインスタンスを生成
	StraightBullet* straightBullet = new StraightBullet();
	if (straightBullet == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!straightBullet->Initialize()) {
		delete straightBullet;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(model);
	straightBullet->model = model;

	//座標をセット
	straightBullet->position = position;

	//速度をセット
	straightBullet->velocity = velocity;

	return straightBullet;
}

void StraightBullet::Update()
{
	//座標を移動
	position += velocity;

	//時間経過で死亡
	if (--deathTimer <= 0) {
		isDead = true;
	}

	//3Dオブジェクトの更新
	ObjObject3d::Update();
}
