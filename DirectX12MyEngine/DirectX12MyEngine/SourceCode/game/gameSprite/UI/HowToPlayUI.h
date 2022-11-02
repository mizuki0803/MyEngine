#pragma once
#include "HowToPlaySprite.h"
#include <memory>
#include <list>

/// <summary>
/// �V�ѕ�UI
/// </summary>
class HowToPlayUI
{
public:
	//���ɕ`�悷��UI
	enum class NextDrawUI {
		Shot,	//�V���b�g
		Charge,	//�`���[�W
		None,	//�Ȃ�
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�V�ѕ�UI</returns>
	static HowToPlayUI* Create();

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �u�V���b�g�v��UI����
	/// </summary>
	void ShotUICreate();

	/// <summary>
	/// �u�`���[�W�v��UI����
	/// </summary>
	void ChargeUICreate();

	//getter
	NextDrawUI GetNextDrawUI() { return nextDrawPhase; }

private: //�����o�ϐ�
	//�V�ѕ��X�v���C�g
	std::list<std::unique_ptr<HowToPlaySprite>> howToPlaySprites;
	//���ɕ`�悷��UI
	NextDrawUI nextDrawPhase = NextDrawUI::Shot;
};