#pragma once
#include "Sprite.h"

/// <summary>
/// �{�X�pHP�o�[
/// </summary>
class BossHPBar : public Sprite
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X�pHP�o�[</returns>
	static BossHPBar* Create(UINT texNumber, const Vector2& position, const int maxHP);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �J�n���̈ړ�
	/// </summary>
	/// <param name="moveTime">�ړ��ɂ����鎞��</param>
	/// <param name="timer">�^�C�}�[</param>
	void StartSetMove(int moveTime, int timer);

	/// <summary>
	/// �J�n���̑傫���ύX
	/// </summary>
	/// <param name="changeTime">�傫���ύX�ɂ����鎞��</param>
	/// <param name="timer">�^�C�}�[</param>
	void StartSetChangeSize(int changeTime,int timer);

	/// <summary>
	/// �o�[�̒����ύX�����Z�b�g
	/// </summary>
	/// <param name="HP">�{�X��HP</param>
	void SetChangeLength(const int HP);

private: //�����o�֐�
	/// <summary>
	/// �o�[�̒����ύX
	/// </summary>
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
	//�\��������W
	Vector2 stayPosition;
};
