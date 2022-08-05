#pragma once
#include "Sprite.h"

/// <summary>
/// �v���C���[�pHP�o�[
/// </summary>
class PlayerHPBar : public Sprite
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�v���C���[�pHP�o�[</returns>
	static PlayerHPBar* Create(UINT texNumber, const Vector2 position, const int maxHP);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �o�[�̒����ύX�����Z�b�g
	/// </summary>
	/// <param name="HP">�v���C���[��HP</param>
	void SetChangeLength(const int HP);


private: //�����o�֐�
	/// <summary>
	/// �o�[�̒����ύX
	/// </summary>
	/// <param name="HPDifference">HP�̍���</param>
	void ChangeLength();


private: //�����o�ϐ�
	//�ő�HP
	int maxHP = 0;
	//�o�[�̒����ύX����
	bool isChangeLength = false;
	//�o�[�̒����ő�l
	const float lengthMax = 400;
	//�o�[�̒����ύX�^�C�}�[
	int changeLengthTimer = 0;
	//�o�[�̒����ύX�O�̒���
	float changeLengthBefore = 0;
	//�o�[�̒����ύX��̒���
	float changeLengthAfter = 0;
};
