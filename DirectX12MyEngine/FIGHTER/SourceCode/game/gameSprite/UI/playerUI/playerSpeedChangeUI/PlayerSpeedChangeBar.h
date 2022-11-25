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
	/// <param name="model">���f��</param>
	/// <returns>�v���C���[���x�ύXUI�o�[</returns>
	static PlayerSpeedChangeBar* Create(UINT texNumber, const Vector2& position, const float maxGaugeNum);

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
	const float lengthMax = 400;
};
