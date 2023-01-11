#include "EnemyDefeatCounter.h"

int EnemyDefeatCounter::defeatCounter = 0;
EnemyDefeatCounter::EnemyDefeatRank EnemyDefeatCounter::scoreRank = EnemyDefeatCounter::EnemyDefeatRank::None;
std::array<int, 2> EnemyDefeatCounter::highScore = {};
std::array<EnemyDefeatCounter::EnemyDefeatRank, 2> EnemyDefeatCounter::bestScoreRank = {
	EnemyDefeatCounter::EnemyDefeatRank::None,
	EnemyDefeatCounter::EnemyDefeatRank::None,
};
std::array<EnemyDefeatCounter::RankBorderline, 2> EnemyDefeatCounter::rankBorderline = {
	70, 40,
	70, 40
};

void EnemyDefeatCounter::AddCounter(int count)
{
	//�J�E���g�𑝂₷
	defeatCounter += count;
}

void EnemyDefeatCounter::CounterReset()
{
	//�J�E���^�[���Z�b�g
	defeatCounter = 0;
	//�����N�����Z�b�g
	scoreRank = EnemyDefeatRank::None;
}

void EnemyDefeatCounter::ScoreConfirm(int stageNum)
{
	//�n�C�X�R�A���X�V�������m�F
	CheckHighScore(stageNum);

	//�����N���m�肳����
	CheckRank(stageNum, defeatCounter);

	//�x�X�g�����N��荡�񃉃��N����Ȃ�x�X�g�����N���X�V����
	if ((int)scoreRank < (int)bestScoreRank[stageNum]) {
		bestScoreRank[stageNum] = scoreRank;
	}
}

void EnemyDefeatCounter::CheckHighScore(int stageNum)
{
	//�J�E���^�[���n�C�X�R�A�ȉ��Ȃ甲����
	if (defeatCounter <= highScore[stageNum]) { return; }

	//�n�C�X�R�A�X�V
	highScore[stageNum] = defeatCounter;
}

void EnemyDefeatCounter::CheckRank(int stageNum, int score)
{
	//�X�R�A���������N���ォ�`�F�b�N
	if (score >= rankBorderline[stageNum].goldRankLine) {
		//�����N�����ɐݒ肵�Ĕ�����
		scoreRank = EnemyDefeatRank::Gold;
		return;
	}
	//�X�R�A���⃉���N���ォ�`�F�b�N
	if (score >= rankBorderline[stageNum].silverRankLine) {
		//�����N����ɐݒ肵�Ĕ�����
		scoreRank = EnemyDefeatRank::Silver;
		return;
	}
	//���ł���ł��Ȃ���΃����N�𓺂ɐݒ�
	scoreRank = EnemyDefeatRank::Bronze;
}
