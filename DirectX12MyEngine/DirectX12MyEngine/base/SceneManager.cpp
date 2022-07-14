#include "SceneManager.h"
#include <cassert>

SceneManager::~SceneManager()
{
	//最後のシーンの終了と解放
	delete scene;
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;

	return &instance;
}

void SceneManager::Update()
{
	//シーン切り替えがあるなら
	if (nextScene)
	{
		//旧シーンがあるなら
		if (scene)
		{
			//旧シーンの終了
			delete scene;
		}
		//シーンを切り替え
		scene = nextScene;
		//次のシーンは空にしておく
		nextScene = nullptr;

		//新しいシーン初期化
		scene->Initialize();
	}

	//毎フレーム更新
	scene->Update();
}

void SceneManager::Draw()
{
	//描画
	scene->Draw();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory);
	assert(nextScene == nullptr);

	//次のシーンをセット
	nextScene = sceneFactory->CreateScene(sceneName);
}
