#include "BossAvatarType01.h"

BossAvatarType01* BossAvatarType01::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//ボス(分身：タイプ01)のインスタンスを生成
	BossAvatarType01* bossAvatarType01 = new BossAvatarType01();
	if (bossAvatarType01 == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	bossAvatarType01->model = model;

	// 初期化
	if (!bossAvatarType01->Initialize()) {
		delete bossAvatarType01;
		assert(0);
		return nullptr;
	}

	//親をセット
	bossAvatarType01->parent = parent;

	//座標をセット
	bossAvatarType01->position = position;

	return bossAvatarType01;
}

void BossAvatarType01::Otamesi()
{
	position.x -= moveSpeed;
}

void BossAvatarType01::Otamesi2()
{
	position.x += moveSpeed;
}

void BossAvatarType01::Otamesi3()
{
	position.y += moveSpeed;
}

void BossAvatarType01::Otamesi4()
{
	position.y -= moveSpeed;
}
