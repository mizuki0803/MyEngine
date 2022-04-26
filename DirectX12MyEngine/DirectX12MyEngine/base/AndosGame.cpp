#include "AndosGame.h"
#include "SceneFactory.h"


void AndosGame::Initialize()
{
	//基底クラスの初期化処理
	FrameWork::Initialize();

	//シーン工場を生成、マネージャーにセット
	sceneFactory = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);

	//シーンマネージャーに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("AIR_RESISTANCE");
}

void AndosGame::Finalize()
{
	//基底クラスの終了処理
	FrameWork::Finalize();
}

void AndosGame::Update()
{
	//基底クラスの更新処理
	FrameWork::Update();
}

void AndosGame::Draw()
{
	//基底クラスの描画処理
	FrameWork::Draw();
}
