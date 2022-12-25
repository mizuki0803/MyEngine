#include "SpaceDustEffect.h"

ObjModel* SpaceDustEffect::spaceDustModel = nullptr;
Player* SpaceDustEffect::player = nullptr;
GameCamera* SpaceDustEffect::gameCamera = nullptr;
const float SpaceDustEffect::spaceDustSize = 0.1f;
bool SpaceDustEffect::isScrollMode = false;

SpaceDustEffect* SpaceDustEffect::Create(const Vector3& position, const Vector3& rotation)
{
	//宇宙塵エフェクトのインスタンスを生成
	SpaceDustEffect* spaceDustEffect = new SpaceDustEffect();
	if (spaceDustEffect == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!spaceDustEffect->Initialize()) {
		delete spaceDustEffect;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(spaceDustModel);
	spaceDustEffect->model = spaceDustModel;
	//座標をセット
	spaceDustEffect->position = position;
	//回転角をセット
	spaceDustEffect->rotation = rotation;
	//大きさをセット
	spaceDustEffect->scale = { spaceDustSize, spaceDustSize, spaceDustSize };

	return spaceDustEffect;
}

void SpaceDustEffect::Update()
{
	//座標を更新
	Vector3 velocity = { 0, 0, -0.05f };
	//スクロール状態なら
	if (isScrollMode) {
		//自機の移動速度状態によって宇宙塵エフェクトをカメラで移動していた速度で動かす
		float scrollSpeed = GameCamera::GetAdvanceSpeed();
		if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::HighSpeed) { scrollSpeed *= GameCamera::GetHighSpeedMagnification(); }
		else if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::SlowSpeed) { scrollSpeed *= GameCamera::GetSlowSpeedMagnification(); }

		velocity.z -= scrollSpeed;
	}
	position += velocity;

	//オブジェクト更新
	ObjObject3d::Update();

	//手前まで行ったら削除
	FrontOfScreenDelete();
}

void SpaceDustEffect::FrontOfScreenDelete()
{
	//座標が自機より手前(画面外手前)まで行ったら削除
	const float flontOfScreenDiffence = 10;
	const float deletePos = gameCamera->GetPosition().z - flontOfScreenDiffence;

	if (position.z <= deletePos) {
		isDelete = true;
	}
}
