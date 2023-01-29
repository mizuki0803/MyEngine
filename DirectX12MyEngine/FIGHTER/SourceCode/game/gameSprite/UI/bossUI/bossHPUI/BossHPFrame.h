#pragma once
#include "Sprite.h"

/// <summary>
/// �{�X�pHP�o�[�t���[��
/// </summary>
class BossHPFrame : public Sprite
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="position">���W</param>
	/// <returns>�{�X�pHP�o�[�t���[��</returns>
	static BossHPFrame* Create(const Texture& texture, const Vector2& position);

public: //�����o�֐�
	/// <summary>
	/// �J�n���̈ړ�
	/// </summary>
	/// <param name="moveTime">�ړ��ɂ����鎞��</param>
	/// <param name="timer">�^�C�}�[</param>
	void StartSetMove(int moveTime, int timer);

	/// <summary>
	/// �J�n���̑傫���ύX
	/// </summary>
	/// <param name="changeTime">�傫���ύX�ɂ����鎞��</param>
	/// <param name="timer">�^�C�}�[</param>
	void StartSetChangeSize(int changeTime, int timer);


private: //�����o�ϐ�
	//�\��������W
	Vector2 stayPosition;
	//�g�̑傫��
	Vector2 frameSize;
};
