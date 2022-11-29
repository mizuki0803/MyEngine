#pragma once
#include "Sprite.h"
#include <memory>
#include <array>

/// <summary>
/// �{�X�o��x�����o
/// </summary>
class BossWarning
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="warningTime">�x������</param>
	/// <returns>�{�X�o��x�����o</returns>
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

	/// <summary>
	/// �e�L�X�g�̐F��ύX��������
	/// </summary>
	void TextColorChange();

private: //�ÓI�����o�ϐ�
	//�x����ʂ̉�ʐԂ̐F�̔Z��
	static const float warningColorAlpha;
	//�x���uWARNING�v�����F(�Ԃ��ő�)
	static const DirectX::XMFLOAT4 warningTextColorMax;
	//�x���uWARNING�v�����F(�Ԃ��ŏ�)
	static const DirectX::XMFLOAT4 warningTextColorMin;

private: //�����o�ϐ�
	//��ʐԗp�X�v���C�g
	std::unique_ptr<Sprite> screenRedSprite;
	//�����X�v���C�g
	std::unique_ptr<Sprite> textSprite;
	//�����w�i�p�X�v���C�g
	std::unique_ptr<Sprite> textBackSprite;
	//�x�����C���X�v���C�g
	std::array<std::unique_ptr<Sprite>, 2> lineSprite;
	//�x�����C���w�i�p�X�v���C�g
	std::array<std::unique_ptr<Sprite>, 2> lineBackSprite;


	//�����X�v���C�g
	std::unique_ptr<Sprite> textSpriteDown;
	//�����w�i�p�X�v���C�g
	std::unique_ptr<Sprite> textBackSpriteDown;
	//�x�����C���X�v���C�g
	std::array<std::unique_ptr<Sprite>, 2> lineSpriteDown;
	//�x�����C���w�i�p�X�v���C�g
	std::array<std::unique_ptr<Sprite>, 2> lineBackSpriteDown;

	//��ʂ𕢂��F�̔Z��
	float colorDeepNum = 0;
	//�e�L�X�g�F�ύX�p�^�C�}�[
	int textColorChangeTimer = 0;
	//�F��Z�����邩
	bool isTextColorDeep = false;
	//�x������
	int32_t warningTime = 0;
	//�x�����ԃ^�C�}�[
	int32_t warningTimer = 0;
	//�폜���邩
	bool isDelete = false;
};
