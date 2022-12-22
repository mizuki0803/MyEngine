#include "BaseStageScene.h"

void BaseStageScene::AddPlayerBullet(std::unique_ptr<PlayerBullet> playerBullet)
{
	//©‹@’eƒŠƒXƒg‚É“o˜^
	playerBullets.push_back(std::move(playerBullet));
}

void BaseStageScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet)
{
	//“G’eƒŠƒXƒg‚É“o˜^
	enemyBullets.push_back(std::move(enemyBullet));
}

void BaseStageScene::AddEnemyBreakEffect(std::unique_ptr<EnemyBreakEffect> enemyBreakEffect)
{
	//“G”j‰óƒGƒtƒFƒNƒgƒŠƒXƒg‚É“o˜^
	enemyBreakEffects.push_back(std::move(enemyBreakEffect));
}
