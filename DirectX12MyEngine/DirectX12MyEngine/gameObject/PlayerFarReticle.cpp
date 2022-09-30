#include "PlayerFarReticle.h"

PlayerFarReticle* PlayerFarReticle::Create(UINT texNumber, float distance, const Vector2& size)
{
	//自機付属の2Dレティクル(遠)簡易作成クラスのインスタンスを生成
	PlayerFarReticle* playerFarReticle = new PlayerFarReticle();
	if (playerFarReticle == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!playerFarReticle->Initialize(texNumber, distance, size)) {
		delete playerFarReticle;
		assert(0);
		return nullptr;
	}

	return playerFarReticle;
}

bool PlayerFarReticle::Initialize(UINT texNumber, float distance, const Vector2& size)
{
	reticle3d.reset(Reticle3D::Create(nullptr, distance));
	reticle2d.reset(PlayerFarReticle2D::Create(texNumber, size));

	return true;
}

void PlayerFarReticle::Update(const XMMATRIX& matWorld, const XMMATRIX& matView, const XMMATRIX& matProjection)
{
	//レティクルに自機のワールド行列を渡す
	reticle3d->SetFollowMatWorld(matWorld);
	reticle3d->Update();
	reticle2d->SetWorldPos3d(reticle3d->GetWorldPos());
	reticle2d->SetMatViewProjection(matView, matProjection);
	reticle2d->Update();
}

void PlayerFarReticle::Draw()
{
	reticle2d->Draw();
}
