#include "AirResistanceScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include <cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;


void AirResistanceScene::Initialize()
{
	//オーディオのインスタンスを取得
	Audio* audio = Audio::GetInstance();
	//サウンドデータ読み込み
	audio->LoadWave("BGM.wav");
	audio->LoadWave("BossDamage.wav");
	//audio->LoadWave("Shot.wav");

	//音全体のボリューム変更
	audio->ChangeVolume(soundVol);


	//カメラ初期化
	camera = new Camera();
	camera->Initialize();


	//ライト生成
	lightGroup = LightGroup::Create();

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);
	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;
	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetPointLightActive(2, false);
	//lightGroup->SetSpotLightActive(0, true);
	lightGroup->SetCircleShadowActive(0, true);



	//objからモデルデータを読み込む
	modelSkydome = Model::LoadFromOBJ("skydome");
	modelGround = Model::LoadFromOBJ("ground");
	modelSphere = Model::LoadFromOBJ("sphere", true);
	modelFighter = Model::LoadFromOBJ("fighter", true);

	// 3Dオブジェクト生成
	objGround = Object3d::Create(modelGround);
	objSkydome = Object3d::Create(modelSkydome);
	objSphere = AirResistance::Create(modelSphere);

	//初期座標
	objGround->SetPosition({ 0, -1, 0 });

	//大きさを変更
	objSkydome->SetScale({ 10,10,10 });
	objGround->SetScale({ 10,10,10 });


	//3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup);
}

void AirResistanceScene::Finalize()
{
	//カメラ解放
	delete camera;

	//ライト解放
	delete lightGroup;

	//モデル解放
	delete modelSkydome;
	delete modelGround;
	delete modelSphere;
	delete modelFighter;

	//3Dオブジェクト解放
	delete objGround;
	delete objSkydome;
	delete objSphere;
}

void AirResistanceScene::Update()
{
	//入力のインスタンスを取得
	Input* input = Input::GetInstance();
	//デバッグテキストのインスタンスを取得
	DebugText* debugText = DebugText::GetInstance();


	//Object3d更新
	objGround->Update();
	objSkydome->Update();
	objSphere->Update();

	{ // imguiからのライトパラメータを反映
	//	lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
	//	lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2], 0 }));
	//	lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));
	//	lightGroup->SetDirLightDir(1, XMVECTOR({ lightDir1[0], lightDir1[1], lightDir1[2], 0 }));
	//	lightGroup->SetDirLightColor(1, XMFLOAT3(lightColor1));
	//	lightGroup->SetDirLightDir(2, XMVECTOR({ lightDir2[0], lightDir2[1], lightDir2[2], 0 }));
	//	lightGroup->SetDirLightColor(2, XMFLOAT3(lightColor2));
	//	lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	//	lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	//	lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
	//	lightGroup->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0], spotLightDir[1], spotLightDir[2], 0 }));
	//	lightGroup->SetSpotLightPos(0, XMFLOAT3(spotLightPos));
	//	lightGroup->SetSpotLightColor(0, XMFLOAT3(spotLightColor));
	//	lightGroup->SetSpotLightAtten(0, XMFLOAT3(spotLightAtten));
	//	lightGroup->SetSpotLightFactorAngleCos(0, XMFLOAT2(spotLightfactorAngleCos));
		lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
		lightGroup->SetCircleShadowCasterPos(0, objSphere->GetPosition());
		lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngleCos(0, XMFLOAT2(circleShadowFactorAngle));
	}

	//ライト更新
	lightGroup->Update();


	//カメラ更新
	XMFLOAT3 spherePos = objSphere->GetPosition();
	camera->SetTarget(spherePos);
	spherePos.z -= 100;
	camera->SetEye(spherePos);
	camera->Update();




	//デバックテキスト
	//X座標,Y座標,縮尺を指定して表示
	debugText->Print("AirResistanceSphere", 100, 50);
	debugText->Print("SPACE KEY:Reset", 100, 100);
	debugText->Print("ENTER KEY:Friction", 100, 120);

	//デバッグ出力
	//sprintf_s(str, "%f\n", soundVol);
	//OutputDebugStringA(str);

	if (input->TriggerKey(DIK_RETURN))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("FRICTION");
	}
}

void AirResistanceScene::Draw()
{
	//Object3d共通コマンド
	Object3d::DrawPrev();
	///-------Object3d描画ここから-------///


	objGround->Draw();
	objSkydome->Draw();
	objSphere->Draw();


	///-------Object3d描画ここまで-------///
}
