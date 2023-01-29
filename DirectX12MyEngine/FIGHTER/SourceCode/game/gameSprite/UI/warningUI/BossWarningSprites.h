#pragma once
#include "Sprite.h"
#include "WarningLine.h"
#include "WarningLineBack.h"
#include "WarningText.h"
#include "WarningTextBack.h"
#include <memory>
#include <array>

/// <summary>
/// �{�X�o��x�����o1���̃X�v���C�g�Q
/// </summary>
class BossWarningSprites
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="leftTopPos">������W</param>
	/// <returns>�{�X�o��x�����o1���̃X�v���C�g�Q</returns>
	static BossWarningSprites* Create(const Vector2& leftTopPos);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="leftTopPos">������W</param>
	/// <returns>����</returns>
	bool Initialize(const Vector2& leftTopPos);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �x��
	/// </summary>
	void Warning();

	/// <summary>
	/// �x���J�n����
	/// </summary>
	/// <param name="warningTimer">�x�����ԃ^�C�}�[</param>
	void WarningStart(float easeTimer);

	/// <summary>
	/// �x���I������
	/// </summary>
	/// <param name="warningTimer">�x�����ԃ^�C�}�[</param>
	void WarningEnd(float easeTimer);

private: //�����o�ϐ�
	//�����X�v���C�g
	std::unique_ptr<WarningText> warningText;
	//�����w�i�p�X�v���C�g
	std::unique_ptr<WarningTextBack> warningTextBack;
	//�x�����C���X�v���C�g
	std::array<std::unique_ptr<WarningLine>, 2> warningLine;
	//�x�����C���w�i�p�X�v���C�g
	std::array<std::unique_ptr<WarningLineBack>, 2> warningLineBack;
	//��ʂ𕢂��F�̔Z��
	float colorDeepNum = 0;
	//�e�L�X�g�F�ύX�p�^�C�}�[
	int textColorChangeTimer = 0;
	//�F��Z�����邩
	bool isTextColorDeep = false;
};
