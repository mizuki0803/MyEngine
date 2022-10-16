#include "StageResultFrameSprite.h"

StageResultFrameSprite* StageResultFrameSprite::Create(UINT texNumber, const Vector2& position, const Vector2& size, const Vector2& texSize)
{
	//�X�e�[�W���U���g�̘g�X�v���C�g�̃C���X�^���X�𐶐�
	StageResultFrameSprite* stageResultFrameSprite = new StageResultFrameSprite();
	if (stageResultFrameSprite == nullptr) {
		return nullptr;
	}

	// ������
	if (!stageResultFrameSprite->Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		delete stageResultFrameSprite;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	stageResultFrameSprite->position = position;

	//�傫�����Z�b�g
	stageResultFrameSprite->size = size;
	stageResultFrameSprite->texSize = texSize;

	return stageResultFrameSprite;
}
