#pragma once
#include "Sprite.h"

/// <summary>
/// �v���C���[�pHP�o�[�t���[��
/// </summary>
class PlayerHPFrame : public Sprite
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�v���C���[�pHP�o�[�t���[��</returns>
	static PlayerHPFrame* Create(UINT texNumber, const Vector2& position);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �񕜃A�C�e���l����Ԃ��J�n
	/// </summary>
	void ItemGetModeStart();

private: //�����o�֐�
	/// <summary>
	/// �񕜃A�C�e���l�����
	/// </summary>
	void ItemGetMode();

private: //�ÓI�����o�ϐ�
	//�ʏ�F
	static const XMFLOAT4 normalColor;
	//�A�C�e���l���F(���F)
	static const XMFLOAT4 itemGetColor;

private: //�����o�ϐ�
	//�񕜃A�C�e���l����Ԃ�
	bool isItemGetMode = false;
	//�񕜃A�C�e���l����ԃ^�C�}�[
	int itemGetModeTimer = 0;
	//�F���ʏ�F��
	bool isNormalColor = true;
};
