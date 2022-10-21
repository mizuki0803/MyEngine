#include "PlayerNearReticle2D.h"

const XMFLOAT4 PlayerNearReticle2D::chargeColor = { 0.9f, 0.9f, 0.1f, 1 };

PlayerNearReticle2D* PlayerNearReticle2D::Create(UINT texNumber, const Vector2& size)
{
	//���@�t����2D���e�B�N��(��)�̃C���X�^���X�𐶐�
	PlayerNearReticle2D* playerNearReticle2D = new PlayerNearReticle2D();
	if (playerNearReticle2D == nullptr) {
		return nullptr;
	}

	// ������
	if (!playerNearReticle2D->Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		delete playerNearReticle2D;
		assert(0);
		return nullptr;
	}

	//�傫�����Z�b�g
	playerNearReticle2D->size = size;

	return playerNearReticle2D;
}

void PlayerNearReticle2D::ChargeModeStart()
{
	//�`���[�W��Ԃ̐F�ɕύX
	color = chargeColor;
}

void PlayerNearReticle2D::ChargeModeEnd()
{
	//�F��ʏ�F�ɖ߂�
	color = normalColor;
}
