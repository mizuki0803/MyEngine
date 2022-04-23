#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "FrictionScene.h"
#include "AirResistanceScene.h"

BaseScene *SceneFactory::CreateScene(const std::string &sceneName)
{
	//éüÇÃÉVÅ[ÉìÇê∂ê¨
	BaseScene *newScene = nullptr;
	
	if(sceneName == "TITLE")
	{
		newScene = new TitleScene();
	}
	else if(sceneName == "GAME")
	{
		newScene = new GameScene();
	}
	else if (sceneName == "FRICTION")
	{
		newScene = new FrictionScene();
	}
	else if (sceneName == "AIR_RESISTANCE")
	{
		newScene = new AirResistanceScene();
	}

	return newScene;
}
