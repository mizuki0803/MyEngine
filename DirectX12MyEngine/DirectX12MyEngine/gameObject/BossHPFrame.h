#pragma once
#include "Sprite.h"

/// <summary>
/// �{�X�pHP�o�[�t���[��
/// </summary>
class BossHPFrame : public Sprite
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X�pHP�o�[�t���[��</returns>
	static BossHPFrame* Create(UINT texNumber, const Vector2 position);
};
