#pragma once
#include "PlayerSpeedChangeBar.h"
#include <memory>

/// <summary>
/// �v���C���[���x�ύXUI
/// </summary>
class PlayerSpeedChangeUI
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�v���C���[���x�ύXUI</returns>
	static PlayerSpeedChangeUI* Create(const Vector2& position, const float maxGaugeNum);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize(const Vector2& position, const float maxGaugeNum);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �o�[�̒����ύX
	/// </summary>
	/// <param name="HP">�v���C���[��HP</param>
	void ChangeLength(const float gaugeNum);

private: //�����o�ϐ�
	//�o�[
	std::unique_ptr<PlayerSpeedChangeBar> speedChangeBar;
	//�t���[��
	std::unique_ptr<Sprite> speedChangeFrame;
};
