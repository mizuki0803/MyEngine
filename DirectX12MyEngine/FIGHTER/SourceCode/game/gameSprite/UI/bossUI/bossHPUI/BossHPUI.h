#pragma once
#include "BossHPLogo.h"
#include "BossHPBar.h"
#include "BossHPFrame.h"
#include <memory>

/// <summary>
/// �{�X�pHPUI
/// </summary>
class BossHPUI
{
	/// <summary>
	/// �J�n���Z�b�g�t�F�[�Y
	/// </summary>
	enum class StartSetPhase
	{
		MoveStayPos,	//��ʊO����ړ�
		ChangeSize,		//�傫���ύX
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="maxHP">�ő�HP</param>
	/// <returns>�{�X�pHPUI</returns>
	static BossHPUI* Create(const Vector2& position, const int maxHP);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
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
	/// <param name="HP">�{�X��HP</param>
	void Damage(const int HP);

private: //�����o�֐�
	/// <summary>
	/// �J�n���̃Z�b�g
	/// </summary>
	void StartSet();

	/// <summary>
	/// �J�n���̈ړ�
	/// </summary>
	void StartSetMove();

	/// <summary>
	/// �J�n���̑傫���ύX
	/// </summary>
	void StartSetChangeSize();


private: //�ÓI�����o�ϐ�
	//�J�n���Z�b�g�s���J��
	static void (BossHPUI::* startSetActionFuncTable[])();

private: //�����o�ϐ�
	//HPUI���S
	std::unique_ptr<BossHPLogo> hpLogo;
	//HP�o�[
	std::unique_ptr<BossHPBar> hpBar;
	//HP�t���[��
	std::unique_ptr<BossHPFrame> hpFrame;
	//�J�n���̃Z�b�g�����邩
	bool isStartSet = true;
	//�J�n���Z�b�g�t�F�[�Y
	StartSetPhase startSetPhase = StartSetPhase::MoveStayPos;
	//�J�n���Z�b�g�p�^�C�}�[
	int32_t startSetTimer = 0;
};
