#include "EnemyDefeatCounter.h"

int EnemyDefeatCounter::defeatCounter = 0;
int EnemyDefeatCounter::highScore = 0;

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

void EnemyDefeatCounter::CheckHighScore()
{
	//�J�E���^�[���n�C�X�R�A�ȉ��Ȃ甲����
	if (defeatCounter <= highScore) { return; }

	//�n�C�X�R�A�X�V
	highScore = defeatCounter;
}
