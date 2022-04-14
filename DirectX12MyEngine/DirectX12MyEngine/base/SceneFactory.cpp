#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "TurretScene.h"

BaseScene *SceneFactory::CreateScene(const std::string &sceneName)
{
	//���̃V�[���𐶐�
	BaseScene *newScene = nullptr;
	
	if(sceneName == "TITLE")
	{
		newScene = new TitleScene();
	}
	else if(sceneName == "GAME")
	{
		newScene = new GameScene();
	}
	else if (sceneName == "TURRET")
	{
		newScene = new TurretScene();
	}

	return newScene;
}
