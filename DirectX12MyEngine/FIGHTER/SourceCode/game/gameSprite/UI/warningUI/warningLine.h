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
	static WarningLine* Create(UINT texNumber, const Vector2& showPos, const bool isMoveRight);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(UINT texNumber, const Vector2& showPos, const bool isMoveRight);

	/// <summary>
	/// �J�n���ɕK�v�ȏ����Z�b�g
	/// </summary>
	/// <param name="createPos">�������W</param>
	void StartSet(const Vector2& createPos);

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
	/// �x�������C���ړ�
	/// </summary>
	void LineMove();

private: //�����o�ϐ�
	//�������W
	Vector2 createPos;
	//�\�����W
	Vector2 showPos;
	//�\���傫��
	Vector2 showSize;
	//�E�Ɉړ����邩
	bool isMoveRight = true;
};