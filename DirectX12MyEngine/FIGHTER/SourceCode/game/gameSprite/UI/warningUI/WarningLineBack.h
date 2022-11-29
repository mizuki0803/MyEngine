#pragma once
#include "Sprite.h"

/// <summary>
/// �x�����C���w�i
/// </summary>
class WarningLineBack : public Sprite
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�x�����C���w�i</returns>
	static WarningLineBack* Create(UINT texNumber, const Vector2& showPos, const Vector2 showSize);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(UINT texNumber, const Vector2& showPos, const Vector2 showSize);

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

	//getter
	const Vector2& GetShowPos() { return showPos; }
	const Vector2& GetShowSize() { return showSize; }

private: //�����o�ϐ�
	//�������W
	Vector2 createPos;
	//�\�����W
	Vector2 showPos;
	//�\���傫��
	Vector2 showSize;
};
