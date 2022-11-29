#pragma once
#include "Sprite.h"
#include "BossWarningSprites.h"
#include <memory>
#include <array>

/// <summary>
/// �{�X�o��x��
/// </summary>
class BossWarning
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="warningTime">�x������</param>
	/// <returns>�{�X�o��x��</returns>
	static BossWarning* Create(int32_t warningTime);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//getter
	bool GetIsDelete() { return isDelete; }

private: //�����o�֐�
	/// <summary>
	/// �x��
	/// </summary>
	void Warning();

	/// <summary>
	/// �x���J�n����
	/// </summary>
	/// <param name="warningTimer">�x�����ԃ^�C�}�[</param>
	void WarningStart(int32_t warningTimer);

	/// <summary>
	/// �x���I������
	/// </summary>
	/// <param name="warningTimer">�x�����ԃ^�C�}�[</param>
	void WarningEnd(int32_t warningTimer);

private: //�ÓI�����o�ϐ�
	//�x����ʂ̉�ʐԂ̐F�̔Z��
	static const float warningColorAlpha;

private: //�����o�ϐ�
	//��ʐԗp�X�v���C�g
	std::unique_ptr<Sprite> screenRedSprite;
	//�x���p�X�v���C�g�Q
	std::array<std::unique_ptr<BossWarningSprites>, 2> bossWarningSprites;
	//��ʂ𕢂��F�̔Z��
	float colorDeepNum = 0;
	//�x������
	int32_t warningTime = 0;
	//�x�����ԃ^�C�}�[
	int32_t warningTimer = 0;
	//�폜���邩
	bool isDelete = false;
};
