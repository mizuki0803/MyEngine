#include "EnemyDefeatCounter.h"

int EnemyDefeatCounter::defeatCounter = 0;
std::array<int, 2> EnemyDefeatCounter::highScore = {};

void EnemyDefeatCounter::AddCounter(int count)
{
	//�J�E���g�𑝂₷
	defeatCounter += count;
}

void EnemyDefeatCounter::CounterReset()
{
	//�J�E���^�[���Z�b�g
	defeatCounter = 0;
}

void EnemyDefeatCounter::CheckHighScore(int stageNum)
{
	//�J�E���^�[���n�C�X�R�A�ȉ��Ȃ甲����
	if (defeatCounter <= highScore[stageNum]) { return; }

	//�n�C�X�R�A�X�V
	highScore[stageNum] = defeatCounter;
}
