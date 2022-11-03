#include "SceneFactory.h"
#include "DebugScene.h"
#include "TitleScene.h"
#include "StageSelectScene.h"
#include "SortieScene.h"
#include "GameScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//éüÇÃÉVÅ[ÉìÇê∂ê¨
	BaseScene* newScene = nullptr;

	if (sceneName == "DEBUG")
	{
		newScene = new DebugScene();
	}
	else if (sceneName == "TITLE")
	{
		newScene = new TitleScene();
	}
	else if (sceneName == "STAGESELECT")
	{
		newScene = new StageSelectScene();
	}
	else if (sceneName == "SORTIE")
	{
		newScene = new SortieScene();
	}
	else if (sceneName == "GAME")
	{
		newScene = new GameScene();
	}

	return newScene;
}
