#pragma once
#include "Sprite.h"

/// <summary>
/// �v���C���[�pHP�o�[�t���[��
/// </summary>
class PlayerHPFrame : public Sprite
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�v���C���[�pHP�o�[�t���[��</returns>
	static PlayerHPFrame* Create(UINT texNumber, const Vector2 position);
};
