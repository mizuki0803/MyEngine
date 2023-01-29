#include "PlayerNearReticle.h"

PlayerNearReticle* PlayerNearReticle::Create(const Texture& texture, float distance, const Vector2& size)
{
	//���@�t����2D���e�B�N��(��)�ȈՍ쐬�N���X�̃C���X�^���X�𐶐�
	PlayerNearReticle* playerNearReticle = new PlayerNearReticle();
	if (playerNearReticle == nullptr) {
		return nullptr;
	}

	// ������
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
	//���e�B�N���Ɏ��@�̃��[���h�s���n��
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
