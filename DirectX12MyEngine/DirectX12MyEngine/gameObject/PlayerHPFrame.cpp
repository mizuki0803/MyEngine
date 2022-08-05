#include "PlayerHPFrame.h"

PlayerHPFrame* PlayerHPFrame::Create(UINT texNumber, const Vector2 position)
{
	//HP�o�[�t���[���̃C���X�^���X�𐶐�
	PlayerHPFrame* playerHPFrame = new PlayerHPFrame();
	if (playerHPFrame == nullptr) {
		return nullptr;
	}

	// ������
	if (!playerHPFrame->Initialize(texNumber, { 0.0f, 0.0f }, false, false)) {
		delete playerHPFrame;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	playerHPFrame->position = position;

	return playerHPFrame;
}
