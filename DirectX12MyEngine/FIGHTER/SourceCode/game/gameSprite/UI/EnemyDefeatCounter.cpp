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
	//カウントを増やす
	defeatCounter += count;
}

void EnemyDefeatCounter::CounterReset()
{
	//カウンターリセット
	defeatCounter = 0;
	//ランクもリセット
	scoreRank = EnemyDefeatRank::None;
}

void EnemyDefeatCounter::ScoreConfirm(int stageNum)
{
	//ハイスコアを更新したか確認
	CheckHighScore(stageNum);

	//ランクを確定させる
	CheckRank(stageNum, defeatCounter);

	//ベストランクより今回ランクが上ならベストランクを更新する
	if ((int)scoreRank < (int)bestScoreRank[stageNum]) {
		bestScoreRank[stageNum] = scoreRank;
	}
}

void EnemyDefeatCounter::CheckHighScore(int stageNum)
{
	//カウンターがハイスコア以下なら抜ける
	if (defeatCounter <= highScore[stageNum]) { return; }

	//ハイスコア更新
	highScore[stageNum] = defeatCounter;
}

void EnemyDefeatCounter::CheckRank(int stageNum, int score)
{
	//スコアが金ランクより上かチェック
	if (score >= rankBorderline[stageNum].goldRankLine) {
		//ランクを金に設定して抜ける
		scoreRank = EnemyDefeatRank::Gold;
		return;
	}
	//スコアが銀ランクより上かチェック
	if (score >= rankBorderline[stageNum].silverRankLine) {
		//ランクを銀に設定して抜ける
		scoreRank = EnemyDefeatRank::Silver;
		return;
	}
	//金でも銀でもなければランクを銅に設定
	scoreRank = EnemyDefeatRank::Bronze;
}
