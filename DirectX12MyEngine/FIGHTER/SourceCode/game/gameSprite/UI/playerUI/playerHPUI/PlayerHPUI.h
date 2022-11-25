#pragma once
#include "PlayerHPBar.h"
#include "PlayerHPFrame.h"
#include <memory>

/// <summary>
/// �v���C���[�pHPUI
/// </summary>
class PlayerHPUI
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�v���C���[�pHPUI</returns>
	static PlayerHPUI* Create(const Vector2& position, const int maxHP);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize(const Vector2& position, const int maxHP);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ��e�����̂Ńo�[�̒����ύX�����Z�b�g
	/// </summary>
	/// <param name="HP">�v���C���[��HP</param>
	void Damage(const int HP);

	/// <summary>
	/// �񕜃A�C�e���l����Ԃ��J�n
	/// </summary>
	void ItemGet(const int HP);

private: //�����o�ϐ�
	//HP�o�[
	std::unique_ptr<PlayerHPBar> hpBar;
	//HP�t���[��
	std::unique_ptr<PlayerHPFrame> hpFrame;
};
