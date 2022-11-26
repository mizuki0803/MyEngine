#pragma once
#include "Sprite.h"

/// <summary>
/// �{�X�pHPUI���S
/// </summary>
class BossHPLogo : public Sprite
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X�pHPUI���S</returns>
	static BossHPLogo* Create(UINT texNumber, const Vector2& position);

public: //�����o�֐�
	/// <summary>
	/// �J�n���̈ړ�
	/// </summary>
	/// <param name="moveTime">�ړ��ɂ����鎞��</param>
	/// <param name="timer">�^�C�}�[</param>
	void StartSetMove(int moveTime, int timer);


private: //�����o�ϐ�
	//�\��������W
	Vector2 stayPosition;
};
