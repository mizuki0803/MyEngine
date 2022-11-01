#pragma once
#include "Sprite.h"
#include <memory>

/// <summary>
/// �V�ѕ�UI
/// </summary>
class HowToPlayUI
{
public:
	//�X�s���t�F�[�Y
	enum class ActionPhase {
		Up,		//��ɏオ��
		Stay,	//��~
		Down,	//���ɉ�����
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�V�ѕ�UI</returns>
	static HowToPlayUI* Create();

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

	//getter
	bool GetIsDead() { return isDead; }

private: //�����o�֐�
	/// <summary>
	/// �e�L�X�g����ɓ�����
	/// </summary>
	void TextUp();

	/// <summary>
	/// �e�L�X�g���~������
	/// </summary>
	void TextStay();

	/// <summary>
	/// �e�L�X�g�����ɓ�����
	/// </summary>
	void TextDown();

private: //�ÓI�����o�ϐ�
	//�s���J��
	static void (HowToPlayUI::* actionFuncTable[])();
	//��~���W
	static const Vector2 stayPos;
	//��ʊO���W
	static const Vector2 outScreenPos;

private: //�����o�ϐ�
	//�`���[�W�����X�v���C�g
	std::unique_ptr<Sprite> chargeHowToPlaySprite;
	//�s��
	ActionPhase phase = ActionPhase::Up;
	//�s���p�^�C�}�[
	int32_t actionTimer = 0;
	//���S�t���O
	bool isDead = false;
};