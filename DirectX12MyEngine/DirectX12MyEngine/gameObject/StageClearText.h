#pragma once
#include "Sprite.h"
#include <array>
#include <memory>

/// <summary>
/// �X�e�[�W�N���A���ɕ\������e�L�X�g
/// </summary>
class StageClearText
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�X�e�[�W�N���A���ɕ\�����镶��</returns>
	static StageClearText* Create();

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize();

	/// <summary>
	///	�X�V
	/// </summary>
	void Update();

	/// <summary>
	///	�`��
	/// </summary>
	void Draw();

private: //�����o�֐�
	/// <summary>
	/// �\�����镶�����X�V
	/// </summary>
	void UpdateDisplayNum();


private: //�ÓI�����o�ϐ�
	static const int textNum = 4;

private: //�����o�ϐ�
	//�X�e�[�W���X�v���C�g
	std::unique_ptr<Sprite> stageNameSprite;
	//�����p�X�v���C�g�z��
	std::unique_ptr<Sprite> textSprites[textNum];
	//�\�����镶����
	int displayTextNum = 0;
	//�\���p�^�C�}�[
	int32_t displayTimer = 0;
};
