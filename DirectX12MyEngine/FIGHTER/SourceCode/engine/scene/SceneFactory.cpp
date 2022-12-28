#include "SceneFactory.h"
#include "DebugScene.h"
#include "TitleScene.h"
#include "StageSelectScene.h"
#include "Stage01SortieScene.h"
#include "Stage01Scene.h"
#include "Stage02Scene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//���̃V�[���𐶐�
	BaseScene* newScene = nullptr;

	if (sceneName == "DEBUG") { newScene = new DebugScene(); }
	else if (sceneName == "TITLE") { newScene = new TitleScene(); }
	else if (sceneName == "STAGESELECT") { newScene = new StageSelectScene(); }
	else if (sceneName == "SORTIE01") { newScene = new Stage01SortieScene(); }
	else if (sceneName == "STAGE01") { newScene = new Stage01Scene(); }
	else if (sceneName == "STAGE02") { newScene = new Stage02Scene(); }

	return newScene;
}
