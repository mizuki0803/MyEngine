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
	//�`�悷��UI
	enum class DrawUI
	{
		Shot,	//�V���b�g
		Charge,	//�`���[�W
		Boost,	//�u�[�X�g
		Brake,	//�u���[�L
		Rolling,//���[�����O
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
	/// UI����
	/// </summary>
	/// <param name="drawUI">�쐬����UI</param>
	/// <param name="nextDrawUI">���ɍ쐬����UI</param>
	void CreateUI(DrawUI drawUI, DrawUI nextDrawUI);

	//getter
	DrawUI GetNextDrawUI() { return nextDrawPhase; }

private: //�����o�ϐ�
	//�V�ѕ��X�v���C�g
	std::list<std::unique_ptr<HowToPlaySprite>> howToPlaySprites;
	//���ɕ`�悷��UI
	DrawUI nextDrawPhase = DrawUI::Shot;
};