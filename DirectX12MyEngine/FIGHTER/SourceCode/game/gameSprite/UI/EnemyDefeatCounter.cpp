#include "EnemyDefeatCounter.h"

int EnemyDefeatCounter::defeatCounter = 0;
std::array<int, 2> EnemyDefeatCounter::highScore = {};

void EnemyDefeatCounter::AddCounter(int count)
{
	//カウントを増やす
	defeatCounter += count;
}

void EnemyDefeatCounter::CounterReset()
{
	//カウンターリセット
	defeatCounter = 0;
}

void EnemyDefeatCounter::CheckHighScore(int stageNum)
{
	//カウンターがハイスコア以下なら抜ける
	if (defeatCounter <= highScore[stageNum]) { return; }

	//ハイスコア更新
	highScore[stageNum] = defeatCounter;
}
