#include "FrameWork.h"
#include "ObjObject3d.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"
#include "LightGroup.h"

void FrameWork::Run()
{
	//ゲーム初期化
	Initialize();

	while (true)	//ゲームループ
	{
		//毎フレーム更新
		Update();

		//終了リクエストが来たらループを抜ける
		if (GetIsEndRequest())
		{
			break;
		}

		//描画
		Draw();
	}

	//ゲーム終了
	Finalize();
}

void FrameWork::Initialize()
{
	//ウインドウ作成
	win = new WindowApp();
	win->WindowCreate(L"ShootingGame");

	//DirectX初期化
	dxbase = new DirectXBase();
	dxbase->Initialize(win);;

	//入力の初期化
	input = Input::GetInstance();
	input->Initialize(win);

	//音声初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	//スプライト共通部分初期化
	spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize(dxbase->GetDevice(), dxbase->GetCmdList(), win->window_width, win->window_height);

	//デバッグテキスト用のテクスチャ番号を指定
	const int debugTextNumber = 0;
	//デバッグテキスト用のテクスチャ読み込み
	spriteCommon->LoadTexture(debugTextNumber, "debugFont.png");
	//デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextNumber);

	//Object3d共通初期化処理
	PostEffect::PostEffectCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	//ポストエフェクトの初期化
	postEffect = PostEffect::Create();

	//objオブジェクト3d共通初期化処理
	ObjObject3d::Object3dCommon(dxbase->GetDevice(), dxbase->GetCmdList());

	//FBXLoader初期化
	FbxLoader::GetInstance()->Initialize(dxbase->GetDevice());

	//FBXオブジェクト3d共通
	FbxObject3d::SetDevice(dxbase->GetDevice());
	FbxObject3d::SetCommandList(dxbase->GetCmdList());
	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	//ライト共通初期化処理
	LightGroup::StaticInitialize(dxbase->GetDevice());

	//パーティクル共通初期化処理
	ParticleManager::ParticleManagerCommon(dxbase->GetDevice(), dxbase->GetCmdList());
}

void FrameWork::Finalize()
{
	//シーン工場解放
	delete sceneFactory;

	//ポストエフェクトの解放
	delete postEffect;

	//FBXLoader解放
	FbxLoader::GetInstance()->Finalize();

	//audio解放
	audio->Finalize();

	//DirectX解放
	delete dxbase;

	//ウインドウ解放
	win->WindowRelease();
	delete win;
	win = nullptr;
}

void FrameWork::Update()
{
	//メッセージ
	//×ボタンで終了メッセージが来たらゲームループを抜ける
	if (win->MessageProc())
	{
		isEndRequest = true;
		return;
	}
	//エスケープキーでゲームループ終了
	if (input->PushKey(DIK_ESCAPE))
	{
		isEndRequest = true;
		return;
	}

	//入力の更新
	input->Update();

	//シーン更新
	SceneManager::GetInstance()->Update();
}

void FrameWork::Draw()
{
	//レンダーテクスチャへの描画
	//postEffect->DrawScenePrev();
	//SceneManager::GetInstance()->Draw();
	//postEffect->DrawSceneRear();

	//グラフィックスコマンド(前)
	dxbase->GraphicsCommandPrev();

	//ポストエフェクトの描画
	//postEffect->Draw();

	//シーン描画
	SceneManager::GetInstance()->Draw();

	//スプライト共通コマンド
	spriteCommon->DrawPrev();
	//デバックテキスト描画
	debugText->DrawAll();

	//グラフィックスコマンド(後)
	dxbase->GraphicsCommandRear();
}
