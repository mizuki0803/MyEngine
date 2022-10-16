#pragma once
#include "NumberSprite.h"
#include "StageResultFrameSprite.h"
#include <vector>
#include <memory>

/// <summary>
/// �X�e�[�W���U���gUI
/// </summary>
class StageResultUI
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�X�e�[�W�N���A��񌋉�UI</returns>
	static StageResultUI* Create(const int enemyDefeatNum, bool isHighScore);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(const int enemyDefeatNum);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//getter
	bool GetIsResultEnd() { return isResultEnd; }

private: //�����o�֐�
	/// <summary>
	/// �\���p���j���X�V
	/// </summary>
	void UpdateDisplayNum();

	/// <summary>
	///	�����X�v���C�g�X�V
	/// </summary>
	void UpdateNumberSprite();

private: //�����o�ϐ�
	//�g�X�v���C�g
	std::unique_ptr<StageResultFrameSprite> frameSprite;
	//�����X�v���C�g
	std::vector<std::unique_ptr<NumberSprite>> numberSprites;
	//�擾�p���j��
	int enemyDefeatNum = 0;
	//�擾�p���j���̌���
	int enemyDefeatNumDigit = 0;
	//�\���p���j��
	int enemyDefeatDisplayNum = 0;
	//�\���p���j���̌���
	int enemyDefeatDisplayNumDigit = 0;
	//�\���p���j���X�V�p�^�C�}�[
	int32_t updateDisplayNumTimer = 0;
	//���U���g��\�����I������
	bool isResultEnd = false;
};