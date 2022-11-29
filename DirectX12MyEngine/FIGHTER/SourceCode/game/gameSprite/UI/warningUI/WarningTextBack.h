#pragma once
#include "Sprite.h"

/// <summary>
/// �x���e�L�X�g�w�i
/// </summary>
class WarningTextBack : public Sprite
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�x���e�L�X�g�w�i</returns>
	static WarningTextBack* Create(UINT texNumber, const Vector2& position, const Vector2 size);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(UINT texNumber, const Vector2& position, const Vector2 size);

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
	const Vector2& GetShowSize() { return showSize; }

private: //�����o�ϐ�
	//�\���傫��
	Vector2 showSize;
};
