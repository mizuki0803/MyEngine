#include "HealingItem.h"
#include "BasePlayer.h"
#include "Easing.h"
#include "ParticleEmitter.h"

BasePlayer* HealingItem::player = nullptr;
const float HealingItem::shineEffectCreateSize = 1.5f;

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

	//キラキラ演出サイズをセット
	healingItem->shineEffectSize = shineEffectCreateSize;

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

	//キラキラ演出
	ShineEffect();

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

	//キラキラ演出サイズを大きく
	shineEffectSize = shineEffectCreateSize;
	//キラキラ演出生成間隔をセット
	shineEffectInterval = shineEffectCreateInterval;
	//キラキラ演出生成数をセット
	shineEffectNum = 5;
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

void HealingItem::ShineEffect()
{
	//接触後一定時間経っていたら抜ける
	const int touchedAfterEffectCreateTime = 30;
	if (touchedTimer >= touchedAfterEffectCreateTime) { return; }

	//キラキラ演出用タイマー更新
	shineEffectTimer++;

	//大きさ変更
	ShineEffectSizeChange();
	//生成間隔変更
	ShineEffectIntervalChange();

	//指定した間隔以外なら抜ける
	if (shineEffectTimer % shineEffectInterval != 0) { return; }

	//キラキラ演出用パーティクル生成
	ParticleEmitter::GetInstance()->ItemShine(position, shineEffectSize, shineEffectNum);

	//生成数変更
	shineEffectNum--;
	shineEffectNum = max(shineEffectNum, 1);
}

void HealingItem::ShineEffectSizeChange()
{
	//最小サイズ
	const float shineEffectSizeMin = 0.8f;
	//既に最小値なら抜ける
	if (shineEffectSize <= shineEffectSizeMin) { return; }

	//1フレームで小さくする値
	const float sizeChangeNum = 0.01f;
	shineEffectSize -= sizeChangeNum;
}

void HealingItem::ShineEffectIntervalChange()
{
	//キラキラ演出生成間隔の最大値
	const int intarvalMax = 15;
	//既に最大値なら抜ける
	if (shineEffectInterval >= intarvalMax) { return; }

	//キラキラ演出生成間隔を変更する時間
	const int shineEffectIntervalChangeTime = 10;
	//指定した間隔以外なら抜ける
	if (shineEffectTimer % shineEffectIntervalChangeTime != 0) { return; }

	//生成間隔を大きくする
	shineEffectInterval++;
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
