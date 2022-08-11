#include "BossHPFrame.h"

BossHPFrame* BossHPFrame::Create(UINT texNumber, const Vector2 position)
{
	//HP�o�[�t���[���̃C���X�^���X�𐶐�
	BossHPFrame* bossHPFrame = new BossHPFrame();
	if (bossHPFrame == nullptr) {
		return nullptr;
	}

	// ������
	if (!bossHPFrame->Initialize(texNumber, { 0.0f, 0.0f }, false, false)) {
		delete bossHPFrame;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	bossHPFrame->position = position;

	return bossHPFrame;
}
