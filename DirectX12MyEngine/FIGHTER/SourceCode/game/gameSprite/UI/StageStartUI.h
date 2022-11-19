#pragma once
#include "Sprite.h"
#include <memory>

/// <summary>
/// �X�e�[�W�J�nUI
/// </summary>
class StageStartUI
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�X�e�[�W�J�nUI</returns>
	static StageStartUI* Create(const int stageNum);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(const int stageNum);

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

private: //�����o�ϐ�
	//�X�e�[�W���X�v���C�g
	std::unique_ptr<Sprite> stageInfoSprite;
	//�����^�C�}�[
	int32_t deadTimer = 0;
	//���S�t���O
	bool isDead = false;
};