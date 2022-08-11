#pragma once
#include "Sprite.h"

/// <summary>
/// �{�X�pHP�o�[
/// </summary>
class BossHPBar : public Sprite
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X�pHP�o�[</returns>
	static BossHPBar* Create(UINT texNumber, const Vector2 position, const int maxHP);

public: //�����o�֐�

	/// <summary>
	/// �o�[�̒����ύX
	/// </summary>
	/// <param name="HP">HP</param>
	void ChangeLength(const int HP);

private: //�����o�ϐ�
	//�ő�HP
	int maxHP = 0;
	//�o�[�̒����ő�l
	const float lengthMax = 400;
};
