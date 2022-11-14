#include "HealingItem.h"
#include "Player.h"
#include "Easing.h"

Player* HealingItem::player = nullptr;

HealingItem* HealingItem::Create(ObjModel* model, const Vector3& position, const float size)
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

	//大きさをセット
	healingItem->scale = { size, size, size };

	return healingItem;
}

void HealingItem::Update()
{
	//ぐるぐる回転
	Round();

	//接触後の動き
	if (isTouched) {
		TouchedAction();
	}

	//3Dオブジェクトの更新
	ObjObject3d::Update();

	//画面外手前まで行ったら削除
	FrontOfScreenDelete();
}

void HealingItem::OnCollision()
{
	//接触した
	isTouched = true;

	//回転の速さをアップ
	const float deadRotSpeed = 15.0f;
	rotSpeed = deadRotSpeed;
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

void HealingItem::Round()
{
	//ぐるぐる回転
	rotation.y -= rotSpeed;
}

void HealingItem::FrontOfScreenDelete()
{
	//座標が自機より手前(画面外手前)まで行ったら削除
	const float flontOfScreenDiffence = 100;
	const float deletePos = player->GetWorldPos().z - flontOfScreenDiffence;
	const Vector3 worldPos = GetWorldPos();

	if (worldPos.z <= deletePos) {
		isDelete = true;
	}
}

void HealingItem::TouchedAction()
{
	//タイマー更新
	const float deadTime = 70;
	const float moveTime = 30;
	touchedTimer++;

	//移動イージングに使用する変数(0～1を算出)
	float easeMoveTime = touchedTimer / moveTime;
	easeMoveTime = min(easeMoveTime, 1);
	//自機の座標に移動する
	position = Easing::LerpVec3(position, player->GetWorldPos(), easeMoveTime);

	//大きさ変更イージングに使用する変数(0～1を算出)
	float easeScaleTime = touchedTimer / deadTime;
	//大きさを変更する
	const float startSize = 2.5f;
	scale.x = Easing::InQuint(startSize, 0, easeScaleTime);
	scale.y = Easing::InQuint(startSize, 0, easeScaleTime);
	scale.z = Easing::InQuint(startSize, 0, easeScaleTime);

	//タイマーが指定した時間になったら
	if (touchedTimer >= deadTime) {
		//削除
		isDelete = true;
	}
}
