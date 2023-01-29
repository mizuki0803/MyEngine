#pragma once
#include "Sprite.h"

/// <summary>
/// �v���C���[���x�ύXUI�o�[
/// </summary>
class PlayerSpeedChangeBar : public Sprite
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="position">���W</param>
	/// <param name="maxGaugeNum">�ő�Q�[�W��</param>
	/// <returns>�v���C���[���x�ύXUI�o�[</returns>
	static PlayerSpeedChangeBar* Create(const Texture& texture, const Vector2& position, const float maxGaugeNum);

public: //�����o�֐�
	/// <summary>
	/// �o�[�̒����ύX
	/// </summary>
	/// <param name="HP">�v���C���[��HP</param>
	void ChangeLength(const float gaugeNum);


private: //�����o�ϐ�
	//�ő�Q�[�W��
	float maxGaugeNum = 0;
	//�o�[�̒����ő�l
	const float lengthMax = 350;
};
