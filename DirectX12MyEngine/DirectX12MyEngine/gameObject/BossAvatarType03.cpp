#include "BossAvatarType03.h"

BossAvatarType03* BossAvatarType03::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//ボス(分身：タイプ03)のインスタンスを生成
	BossAvatarType03* bossAvatarType03 = new BossAvatarType03();
	if (bossAvatarType03 == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	bossAvatarType03->model = model;

	// 初期化
	if (!bossAvatarType03->Initialize()) {
		delete bossAvatarType03;
		assert(0);
		return nullptr;
	}

	//親をセット
	bossAvatarType03->parent = parent;

	//座標をセット
	bossAvatarType03->position = position;

	return bossAvatarType03;
}

void BossAvatarType03::Otamesi()
{
	position.y -= moveSpeed;
}

void BossAvatarType03::Otamesi2()
{
	position.y += moveSpeed;
}

void BossAvatarType03::Otamesi3()
{
	position.x -= moveSpeed;
}

void BossAvatarType03::Otamesi4()
{
	position.x += moveSpeed;
}
