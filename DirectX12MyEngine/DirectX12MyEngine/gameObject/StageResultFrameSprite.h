#pragma once
#include "Sprite.h"

/// <summary>
/// �X�e�[�W���U���g�̘g�X�v���C�g
/// </summary>
class StageResultFrameSprite : public Sprite
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�X�e�[�W���U���g�̘g�X�v���C�g</returns>
	static StageResultFrameSprite* Create(UINT texNumber, const Vector2& position, const Vector2& size, const Vector2& texSize);
};