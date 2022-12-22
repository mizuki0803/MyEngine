#include "BaseStageScene.h"

void BaseStageScene::AddPlayerBullet(std::unique_ptr<PlayerBullet> playerBullet)
{
	//���@�e���X�g�ɓo�^
	playerBullets.push_back(std::move(playerBullet));
}

void BaseStageScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet)
{
	//�G�e���X�g�ɓo�^
	enemyBullets.push_back(std::move(enemyBullet));
}

void BaseStageScene::AddEnemyBreakEffect(std::unique_ptr<EnemyBreakEffect> enemyBreakEffect)
{
	//�G�j��G�t�F�N�g���X�g�ɓo�^
	enemyBreakEffects.push_back(std::move(enemyBreakEffect));
}
