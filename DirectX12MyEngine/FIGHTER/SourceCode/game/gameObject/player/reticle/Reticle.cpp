#include "Reticle.h"

Reticle* Reticle::Create(UINT texNumber, float distance, const Vector2& size)
{
	//レティクルのインスタンスを生成
	Reticle* reticle = new Reticle();
	if (reticle == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!reticle->Initialize(texNumber, distance, size)) {
		delete reticle;
		assert(0);
		return nullptr;
	}

	return reticle;
}

bool Reticle::Initialize(UINT texNumber, float distance, const Vector2& size)
{
	reticle3d.reset(Reticle3D::Create(nullptr, distance));
	reticle2d.reset(Reticle2D::Create(texNumber, size));

	return true;
}

void Reticle::Update(const XMMATRIX& matWorld, const XMMATRIX& matView, const XMMATRIX& matProjection)
{
	//レティクルに自機のワールド行列を渡す
	reticle3d->SetFollowMatWorld(matWorld);
	reticle3d->Update();
	reticle2d->SetWorldPos3d(reticle3d->GetWorldPos());
	reticle2d->SetMatViewProjection(matView, matProjection);
	reticle2d->Update();
}

void Reticle::Draw()
{
	reticle2d->Draw();
}
