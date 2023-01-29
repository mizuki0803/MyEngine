#pragma once
#include "Sprite.h"

/// <summary>
/// �V�ѕ��X�v���C�g
/// </summary>
class HowToPlaySprite : public Sprite
{
public:
	//�s���t�F�[�Y
	enum class ActionPhase {
		Up,		//��ɏオ��
		Stay,	//��~
		Down,	//���ɉ�����
	};

public:
	/// <summary>
	/// ��������
	/// </summary>	
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="size">�傫��</param>
	/// <param name="texSize">�e�N�X�`���T�C�Y</param>
	/// <param name="leftTop">������W</param>
	/// <returns>�V�ѕ��X�v���C�g</returns>
	static HowToPlaySprite* Create(const Texture& texture, const Vector2& size, const Vector2& texSize, const Vector2& leftTop);

public: //�����o�ϐ�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

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
	static void (HowToPlaySprite::* actionFuncTable[])();
	//��~���W
	static const Vector2 stayPos;
	//��ʊO���W
	static const Vector2 outScreenPos;

private: //�����o�ϐ�
	//�s��
	ActionPhase phase = ActionPhase::Up;
	//�s���p�^�C�}�[
	int32_t actionTimer = 0;
	//���S�t���O
	bool isDead = false;
};