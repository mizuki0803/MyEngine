#pragma once
#include "Sprite.h"
#include <memory>

/// <summary>
/// ���@�_���[�W���o
/// </summary>
class PlayerDamageEffect
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>���@�_���[�W���o</returns>
	static PlayerDamageEffect* Create();

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

	/// <summary>
	/// �_���[�W���o���J�n����
	/// </summary>
	/// <param name="maxHP">�ő�HP</param>
	/// <param name="HP">HP</param>
	void DamageEffectStart(const int maxHP, const int HP);

private: //�����o�֐�
	/// <summary>
	/// �_���[�W���o
	/// </summary>
	void DamageEffect();

private: //�����o�ϐ�
	//���o�p�X�v���C�g
	std::unique_ptr<Sprite> effectSprite;
	//��ʂ𕢂��F�̔Z��
	float colorDeepNum = 0;
	//�`�掞�ԃ^�C�}�[
	int32_t drawTimer = 0;
	//�`�悷�邩
	bool isDraw = false;
};
