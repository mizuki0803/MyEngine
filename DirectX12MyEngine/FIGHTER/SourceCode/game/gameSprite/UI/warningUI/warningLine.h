#pragma once
#include "Sprite.h"

/// <summary>
/// �x�����C��
/// </summary>
class WarningLine : public Sprite
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�x�����C��</returns>
	static WarningLine* Create(const Vector2& createPos, const Vector2& stayPos);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize();

private: //�����o�֐�
	/// <summary>
	/// �x���J�n����
	/// </summary>
	/// <param name="warningTimer">�x�����ԃ^�C�}�[</param>
	void WarningStart(int32_t warningTimer);

	/// <summary>
	/// �x����
	/// </summary>
	void Warning();

	/// <summary>
	/// �x���I������
	/// </summary>
	/// <param name="warningTimer">�x�����ԃ^�C�}�[</param>
	void WarningEnd(int32_t warningTimer);

private: //�����o�ϐ�
	//�x������
	int32_t warningTime = 0;
	//�x�����ԃ^�C�}�[
	int32_t warningTimer = 0;
};
