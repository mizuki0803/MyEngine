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
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="position">���W</param>
	/// <param name="size">�傫��</param>
	/// <returns>�x���e�L�X�g�w�i</returns>
	static WarningTextBack* Create(const Texture& texture, const Vector2& position, const Vector2 size);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="position">���W</param>
	/// <param name="size">�傫��</param>
	/// <returns>����</returns>
	bool Initialize(const Texture& texture, const Vector2& position, const Vector2 size);

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
