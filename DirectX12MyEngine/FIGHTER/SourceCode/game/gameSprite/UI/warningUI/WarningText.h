#pragma once
#include "Sprite.h"

/// <summary>
/// �x���e�L�X�g�uWARNING�v
/// </summary>
class WarningText : public Sprite
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�x���e�L�X�g�uWARNING�v</returns>
	static WarningText* Create(UINT texNumber, const Vector2& position);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(UINT texNumber, const Vector2& position);

	/// <summary>
	/// �x���J�n����
	/// </summary>
	/// <param name="easeTimer">�C�[�W���O�^�C�}�[(0�`1)/param>
	void WarningStart(float easeTimer);

	/// <summary>
	/// �x���I������
	/// </summary>
	/// <param name="easeTimer">�C�[�W���O�^�C�}�[(0�`1)</param>
	void WarningEnd(float easeTimer);

	/// <summary>
	/// �e�L�X�g�̐F��ύX
	/// </summary>
	void TextColorChange();

	//getter
	const Vector2& GetShowSize() { return showSize; }

private: //�ÓI�����o�ϐ�
	//�x���uWARNING�v�����F(�Ԃ��ő�)
	static const DirectX::XMFLOAT4 warningTextColorMax;
	//�x���uWARNING�v�����F(�Ԃ��ŏ�)
	static const DirectX::XMFLOAT4 warningTextColorMin;

private: //�����o�ϐ�
	//�\���傫��
	Vector2 showSize;
	//�e�L�X�g�F�ύX�p�^�C�}�[
	int textColorChangeTimer = 0;
	//�F��Z�����邩
	bool isTextColorDeep = false;
};
