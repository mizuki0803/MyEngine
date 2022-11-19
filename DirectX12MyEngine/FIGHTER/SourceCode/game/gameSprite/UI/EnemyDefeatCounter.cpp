#include "EnemyDefeatCounter.h"

int EnemyDefeatCounter::defeatCounter = 0;
int EnemyDefeatCounter::highScore = 0;

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

void EnemyDefeatCounter::CheckHighScore()
{
	//カウンターがハイスコア以下なら抜ける
	if (defeatCounter <= highScore) { return; }

	//ハイスコア更新
	highScore = defeatCounter;
}
