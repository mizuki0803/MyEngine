#include "SceneFactory.h"
#include "DebugScene.h"
#include "GameScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//���̃V�[���𐶐�
	BaseScene* newScene = nullptr;

	if (sceneName == "DEBUG")
	{
		newScene = new DebugScene();
	}
	else if (sceneName == "GAME")
	{
		newScene = new GameScene();
	}

	return newScene;
}
