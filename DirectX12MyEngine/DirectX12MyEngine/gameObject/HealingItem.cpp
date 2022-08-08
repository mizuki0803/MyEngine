#include "HealingItem.h"
#include "Player.h"

Player* HealingItem::player = nullptr;

HealingItem* HealingItem::Create(ObjModel* model, const Vector3& position)
{
	//回復アイテムのインスタンスを生成
	HealingItem* healingItem = new HealingItem();
	if (healingItem == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	healingItem->model = model;

	// 初期化
	if (!healingItem->Initialize()) {
		delete healingItem;
		assert(0);
		return nullptr;
	}

	//座標をセット
	healingItem->position = position;

	//デバッグ用
	healingItem->color = { 1,0,0,1 };
	healingItem->scale = { 0.5f, 0.5f, 0.5f };

	return healingItem;
}

void HealingItem::Update()
{
	//3Dオブジェクトの更新
	ObjObject3d::Update();

	//画面外手前まで行ったら削除
	FrontOfScreenDelete();
}

void HealingItem::OnCollision()
{
    //死亡させる
    isDead = true;
}

Vector3 HealingItem::GetWorldPos()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//平行移動成分を取得
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void HealingItem::FrontOfScreenDelete()
{
	//座標が自機より手前(画面外手前)まで行ったら削除
	const float flontOfScreenDiffence = 100;
	const float deletePos = player->GetWorldPos().z - flontOfScreenDiffence;
	const Vector3 worldPos = GetWorldPos();

	if (worldPos.z <= deletePos) {
		isDead = true;
	}
}
