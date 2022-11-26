#pragma once
#include "Sprite.h"
#include <memory>
#include <array>

/// <summary>
/// �{�X���\��UI
/// </summary>
class BossNameUI
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="bossType">�{�X�̎��</param>
	/// <returns>�{�X���\��UI</returns>
	static BossNameUI* Create(int bossType);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="bossType">�{�X�̎��</param>
	/// <returns>����</returns>
	bool Initialize(int bossType);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();


private: //�����o�֐�
	/// <summary>
	/// �\�����ԃJ�E���g
	/// </summary>
	void CountDrawTimer();


private: //�ÓI�����o�ϐ�
	static const int spriteNum = 2;

private: //�����o�ϐ�
	//�X�v���C�g(������\�������array�ɂ��Ă��� ��� : 0�A���O : 1)
	std::array<std::unique_ptr<Sprite>, spriteNum> sprites;
	//�{�X�̎��
	int bossType = 0;
	//�\�����ԃ^�C�}�[
	int32_t drawTimer = 0;
	//�\���t���O
	bool isDraw = true;
};
