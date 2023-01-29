#include "PlayerFarReticle.h"

PlayerFarReticle* PlayerFarReticle::Create(const Texture& texture, float distance, const Vector2& normalSize, const Vector2& chargeModeSize)
{
	//���@�t����2D���e�B�N��(��)�ȈՍ쐬�N���X�̃C���X�^���X�𐶐�
	PlayerFarReticle* playerFarReticle = new PlayerFarReticle();
	if (playerFarReticle == nullptr) {
		return nullptr;
	}

	// ������
	if (!playerFarReticle->Initialize(texture, distance, normalSize, chargeModeSize)) {
		delete playerFarReticle;
		assert(0);
		return nullptr;
	}

	return playerFarReticle;
}

bool PlayerFarReticle::Initialize(const Texture& texture, float distance, const Vector2& normalSize, const Vector2& chargeModeSize)
{
	reticle3d.reset(Reticle3D::Create(nullptr, distance));
	reticle2d.reset(PlayerFarReticle2D::Create(texture, normalSize, chargeModeSize));

	return true;
}

void PlayerFarReticle::Update(const XMMATRIX& matWorld, const XMMATRIX& matView, const XMMATRIX& matProjection)
{
	//���e�B�N���Ɏ��@�̃��[���h�s���n��
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
