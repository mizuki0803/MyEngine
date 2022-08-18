#include "BossAvatarType04.h"

BossAvatarType04* BossAvatarType04::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//ボス(分身：タイプ04)のインスタンスを生成
	BossAvatarType04* bossAvatarType04 = new BossAvatarType04();
	if (bossAvatarType04 == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	bossAvatarType04->model = model;

	// 初期化
	if (!bossAvatarType04->Initialize()) {
		delete bossAvatarType04;
		assert(0);
		return nullptr;
	}

	//親をセット
	bossAvatarType04->parent = parent;

	//座標をセット
	bossAvatarType04->position = position;

	return bossAvatarType04;
}

void BossAvatarType04::Otamesi()
{
	position.y += moveSpeed;
}

void BossAvatarType04::Otamesi2()
{
	position.y -= moveSpeed;
}

void BossAvatarType04::Otamesi3()
{
	position.x += moveSpeed;
}

void BossAvatarType04::Otamesi4()
{
	position.x -= moveSpeed;
}
