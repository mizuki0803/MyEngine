#pragma once
#include "Sprite.h"
#include <memory>

/// <summary>
/// �^�C�g��UI
/// </summary>
class TitleUI
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�^�C�g��UI</returns>
	static TitleUI* Create();

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

private: //�����o�֐�
	/// <summary>
	/// �{�^���\�������邵�Ȃ���؂�ւ�
	/// </summary>
	void DrawChangeButtonSprite();

private: //�����o�ϐ�
	//�^�C�g�����S�X�v���C�g
	std::unique_ptr<Sprite> titleSprite;
	//B�{�^���X�v���C�g
	std::unique_ptr<Sprite> bButtonSprite;
	//�{�^���X�v���C�g��`�悷�邩
	bool isDrawButtonSprite = true;
	//�`��p�^�C�}�[
	int32_t drawTimer = 0;
};