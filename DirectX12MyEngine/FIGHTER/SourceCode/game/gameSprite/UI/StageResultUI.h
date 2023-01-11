#pragma once
#include "NumberSprite.h"
#include <vector>
#include <memory>

/// <summary>
/// �X�e�[�W���U���gUI
/// </summary>
class StageResultUI
{
private: //���U���g�s���̃t�F�[�Y
	enum class ResultActionPhase {
		UpdateDisplayNum,	//�\���p�����X�V
		RankMedalSet,		//�X�R�A�ɉ������F�̃��_�����Z�b�g
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="enemyDefeatNum">�G���j��</param>
	/// <param name="enemyDefeatRank">�����N</param>
	/// <returns>�X�e�[�W�N���A��񌋉�UI</returns>
	static StageResultUI* Create(const int enemyDefeatNum, const int enemyDefeatRank);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(const int enemyDefeatNum, const int enemyDefeatRank);

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
	bool GetIsDrawButtonSprite() { return isDrawButtonSprite; }

	//setter
	void SetIsDrawButtonSprite(bool isDraw) { isDrawButtonSprite = isDraw; }

private: //�����o�֐�
	/// <summary>
	/// �\���p���j���X�V
	/// </summary>
	void UpdateDisplayNum();

	/// <summary>
	///	�����X�v���C�g�X�V
	/// </summary>
	void UpdateNumberSprite();

	/// <summary>
	/// �����N�ɉ����ă��_�����Z�b�g����s��
	/// </summary>
	void RankMedalSetAction();

private: //�ÓI�����o�ϐ�
	//���U���g�̍s���J��
	static void (StageResultUI::* resultActionPhaseFuncTable[])();

private: //�����o�ϐ�
	//�g�X�v���C�g
	std::unique_ptr<Sprite> frameSprite;
	//�����X�v���C�g
	std::vector<std::unique_ptr<NumberSprite>> numberSprites;
	//�����N���_���X�v���C�g
	std::unique_ptr<Sprite> rankMedalSprite;
	//B�{�^���X�v���C�g
	std::unique_ptr<Sprite> bButtonSprite;
	//���U���g�s��
	ResultActionPhase resultActionPhase = ResultActionPhase::UpdateDisplayNum;
	//�s���p�^�C�}�[
	int32_t actionTimer = 0;
	//�擾�p���j��
	int enemyDefeatNum = 0;
	//�擾�p���j���̌���
	int enemyDefeatNumDigit = 0;
	//�\���p���j��
	int enemyDefeatDisplayNum = 0;
	//�\���p���j���̌���
	int enemyDefeatDisplayNumDigit = 0;
	//���U���g��\�����I������
	bool isResultEnd = false;
	//�{�^���X�v���C�g��`�悷�邩
	bool isDrawButtonSprite = false;
};