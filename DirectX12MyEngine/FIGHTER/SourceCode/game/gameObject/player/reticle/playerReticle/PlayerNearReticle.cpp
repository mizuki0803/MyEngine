#include "PlayerNearReticle.h"

PlayerNearReticle* PlayerNearReticle::Create(const Texture& texture, float distance, const Vector2& size)
{
	//自機付属の2Dレティクル(近)簡易作成クラスのインスタンスを生成
	PlayerNearReticle* playerNearReticle = new PlayerNearReticle();
	if (playerNearReticle == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!playerNearReticle->Initialize(texture, distance, size)) {
		delete playerNearReticle;
		assert(0);
		return nullptr;
	}

	return playerNearReticle;
}

bool PlayerNearReticle::Initialize(const Texture& texture, float distance, const Vector2& size)
{
	reticle3d.reset(Reticle3D::Create(nullptr, distance));
	reticle2d.reset(PlayerNearReticle2D::Create(texture, size));

	return true;
}

void PlayerNearReticle::Update(const XMMATRIX& matWorld, const XMMATRIX& matView, const XMMATRIX& matProjection)
{
	//レティクルに自機のワールド行列を渡す
	reticle3d->SetFollowMatWorld(matWorld);
	reticle3d->Update();
	reticle2d->SetWorldPos3d(reticle3d->GetWorldPos());
	reticle2d->SetMatViewProjection(matView, matProjection);
	reticle2d->Update();
}

void PlayerNearReticle::Draw()
{
	reticle2d->Draw();
}
