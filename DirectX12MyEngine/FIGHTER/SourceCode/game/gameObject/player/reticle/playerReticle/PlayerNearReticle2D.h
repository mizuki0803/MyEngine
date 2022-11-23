#pragma once
#include "Reticle2D.h"

/// <summary>
/// ���@�t����2D���e�B�N��(��)
/// </summary>
class PlayerNearReticle2D : public Reticle2D
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>���@�t����2D���e�B�N��(��)</returns>
	static PlayerNearReticle2D* Create(UINT texNumber, const Vector2& size);

public: //�����o�֐�
	/// <summary>
	/// �`���[�W��Ԃ��J�n����
	/// </summary>
	void ChargeModeStart();

	/// <summary>
	/// �`���[�W��Ԃ��I������
	/// </summary>
	void ChargeModeEnd();

private: //�ÓI�����o�ϐ�
	//�`���[�W��Ԏ��̐F
	static const XMFLOAT4 chargeColor;
};