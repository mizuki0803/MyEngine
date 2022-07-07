#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "FbxLoader.h"
#include <cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;


void TitleScene::Initialize()
{
	//カメラ初期化
	camera = new Camera();
	camera->Initialize();
	camera->SetTarget({ 0, 0, 0 });
	camera->SetDistance(8.0f);

	//ライト生成
	lightGroup = LightGroup::Create();

	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;
	lightGroup->SetPointLightActive(0, true);
	lightGroup->SetPointLightActive(1, true);
	lightGroup->SetPointLightActive(2, true);
	//lightGroup->SetSpotLightActive(0, true);
	lightGroup->SetCircleShadowActive(0, true);

	//objからモデルデータを読み込む
	modelSphere = ObjModel::LoadFromOBJ("sphere", true);

	//objオブジェクト生成
	objSphere = ObjObject3d::Create(modelSphere);

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(camera);
	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup);


	//FBXオブジェクトのカメラをセット
	FbxObject3d::SetCamera(camera);

	//モデル名を指定してファイル読み込み
	//FbxLoader::GetInstance()->LoadModelFromFile("cube");
	//fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	//オブジェクト生成とFBXモデルのセット
	fbxObject1 = new FbxObject3d;
	fbxObject1->Initialize();
	fbxObject1->SetFbxModel(fbxModel1);
	fbxObject1->PlayAnimation();
	fbxObject1->SetRotation({ 0, 90, 0 });
}

void TitleScene::Finalize()
{
	//カメラ解放
	delete camera;

	//ライト解放
	delete lightGroup;

	//モデル解放
	delete modelSphere;

	//objオブジェクト解放
	delete objSphere;

	//FBXオブジェクト解放
	delete fbxModel1;
	delete fbxObject1;
}

void TitleScene::Update()
{
	//入力のインスタンスを取得
	Input* input = Input::GetInstance();
	//デバッグテキストのインスタンスを取得
	DebugText* debugText = DebugText::GetInstance();

	{ // imguiからのライトパラメータを反映
	//	lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
	//	lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2], 0 }));
	//	lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));
	//	lightGroup->SetDirLightDir(1, XMVECTOR({ lightDir1[0], lightDir1[1], lightDir1[2], 0 }));
	//	lightGroup->SetDirLightColor(1, XMFLOAT3(lightColor1));
	//	lightGroup->SetDirLightDir(2, XMVECTOR({ lightDir2[0], lightDir2[1], lightDir2[2], 0 }));
	//	lightGroup->SetDirLightColor(2, XMFLOAT3(lightColor2));
		lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos));
		lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor));
		lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
		//	lightGroup->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0], spotLightDir[1], spotLightDir[2], 0 }));
		//	lightGroup->SetSpotLightPos(0, XMFLOAT3(spotLightPos));
		//	lightGroup->SetSpotLightColor(0, XMFLOAT3(spotLightColor));
		//	lightGroup->SetSpotLightAtten(0, XMFLOAT3(spotLightAtten));
		//	lightGroup->SetSpotLightFactorAngleCos(0, XMFLOAT2(spotLightfactorAngleCos));
		lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
		lightGroup->SetCircleShadowCasterPos(0, fbxObject1->GetPosition());
		lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngleCos(0, XMFLOAT2(circleShadowFactorAngle));
	}

	//ライト更新
	lightGroup->Update();


	// カメラ移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_E) || input->PushKey(DIK_C))
	{
		XMFLOAT3 move = { 0, 0, 0 };
		if (input->PushKey(DIK_W)) { move.y += 0.1f; }
		else if (input->PushKey(DIK_S)) { move.y -= 0.1f; }
		if (input->PushKey(DIK_D)) { move.x += 0.1f; }
		else if (input->PushKey(DIK_A)) { move.x -= 0.1f; }
		if (input->PushKey(DIK_E)) { move.z += 0.1f; }
		else if (input->PushKey(DIK_C)) { move.z -= 0.1f; }
		camera->MoveVector(move);
	}


	//カメラのアングルを変更する
	if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		XMFLOAT3 angle = camera->GetAngle();
		if (input->PushKey(DIK_LEFT)) { angle.y += 1.0f; }
		if (input->PushKey(DIK_RIGHT)) { angle.y -= 1.0f; }
		if (input->PushKey(DIK_UP)) { angle.x += 1.0f; }
		if (input->PushKey(DIK_DOWN)) { angle.x -= 1.0f; }
		camera->SetAngle(angle);
	}

	//カメラの距離を変更する
	if (input->PushKey(DIK_M) || input->PushKey(DIK_N))
	{
		float dis = camera->GetDistance();
		if (input->PushKey(DIK_M)) { dis += 0.1f; }
		if (input->PushKey(DIK_N)) { dis -= 0.1f; }
		camera->SetDistance(dis);
	}

	//カメラ更新
	camera->Update();

	//Object3d更新
	objSphere->Update();

	fbxObject1->Update();

	//デバックテキスト
	//X座標,Y座標,縮尺を指定して表示
	debugText->Print("SCENE1", 50, 50);
	debugText->Print("[1] monochromatic", 50, 70);
	debugText->Print("[2] ADS", 50, 90);
	debugText->Print("[3] Toon", 50, 110);

	if (input->TriggerKey(DIK_RETURN))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::Draw()
{
	//Object3d共通コマンド
	ObjObject3d::DrawPrev();
	///-------Object3d描画ここから-------///


	objSphere->Draw();

	//fbxObject1->Draw();


	///-------Object3d描画ここまで-------///
}

