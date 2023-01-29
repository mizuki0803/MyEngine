#pragma once
#include <array>

/// <summary>
/// �G��|�������J�E���^�[
/// </summary>
class EnemyDefeatCounter
{
public: //enum
	//�X�R�A�����N
	enum class EnemyDefeatRank {
		Gold,	//��
		Silver,	//��
		Bronze,	//��
		None,	//���ݒ�
	};

public:
	//�����N�̃{�[�_�[���C��
	struct RankBorderline {
		int goldRankLine = 0;	//�����N���̃{�[�_�[���C��
		int silverRankLine = 0;	//�����N��̃{�[�_�[���C��
	};


public: //�ÓI�����o�֐�
	/// <summary>
	/// �J�E���g�𑝂₷
	/// </summary>
	/// <param name="count">���₷��</param>
	static void AddCounter(int count = 1);

	/// <summary>
	/// �J�E���^�[���Z�b�g
	/// </summary>
	static void CounterReset();

	/// <summary>
	/// �X�R�A���m�肳����
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	static void ScoreConfirm(int stageNum);

	//getter
	static int GetDefeatCount() { return defeatCounter; }
	static EnemyDefeatRank GetRank() { return scoreRank; }
	static int GetHighScore(int stageNum) { return highScore[stageNum]; }
	static EnemyDefeatRank GetBestRank(int stageNum) { return bestScoreRank[stageNum]; }

private: //�����o�֐�
	/// <summary>
	/// �n�C�X�R�A�X�V���m�F
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	static void CheckHighScore(int stageNum);

	/// <summary>
	/// �ǂ̃����N���`�F�b�N
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	/// <param name="score">���j�X�R�A</param>
	static void CheckRank(int stageNum, int score);

private: //�ÓI�����o�ϐ�
	//�J�E���^�[
	static int defeatCounter;
	//�����N
	static EnemyDefeatRank scoreRank;
	//�n�C�X�R�A
	static std::array<int, 2> highScore;
	//�x�X�g�����N
	static std::array<EnemyDefeatRank, 2> bestScoreRank;
	//�����N�̃{�[�_�[���C��
	static std::array<RankBorderline, 2> rankBorderline;
};