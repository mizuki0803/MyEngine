#include "TurretScene.h"
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


void TurretScene::Initialize()
{
	//オーディオのインスタンスを取得
	Audio* audio = Audio::GetInstance();
	//サウンドデータ読み込み
	audio->LoadWave("BGM.wav");
	audio->LoadWave("BossDamage.wav");
	//audio->LoadWave("Shot.wav");

	//音全体のボリューム変更
	audio->ChangeVolume(soundVol);
	//audio->PlayWave("BGM.wav", true);


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


	//パーティクル生成
	ParticleManager::SetCamera(camera);
	particleMan = ParticleManager::Create();

	//スプライト共通部分のインスタンスを取得
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	//スプライト用テクスチャ読み込み
	spriteCommon->LoadTexture(1, "pad1.png");
	spriteCommon->LoadTexture(2, "mario.jpg");

	//スプライト生成
	sprite = Sprite::Create(2, { 0, 0 });
	sprite2 = Sprite::Create(1);

	//スプライト座標移動
	sprite->SetPosition({ 100, 100 });
	sprite2->SetPosition({ 800, 300 });
	//スプライト回転
	sprite2->SetRotation(90);
	//スプライトサイズ変更
	sprite->SetSize({ 100, 100 });
	sprite2->SetSize({ 428, 150 });
	//スプライトテクスチャサイズ変更
	sprite->SetTexSize({ 64, 478 });
	sprite2->SetTexSize({ 428, 150 });
	//スプライト色変更
	sprite2->SetColor({ 1, 0, 0, 0.8f });


	//objからモデルデータを読み込む
	modelSkydome = Model::LoadFromOBJ("skydome");
	modelGround = Model::LoadFromOBJ("ground");
	modelSphere = Model::LoadFromOBJ("sphere", true);
	modelFighter = Model::LoadFromOBJ("fighter", true);

	// 3Dオブジェクト生成
	objGround = Object3d::Create(modelGround);
	objSkydome = Object3d::Create(modelSkydome);
	objTurret = Turret::Create(modelFighter, modelSphere);

	//初期座標
	objGround->SetPosition({ 0, -1, 0 });

	//サイズ変更
	objGround->SetScale({ 20, 20, 20 });
	objSkydome->SetScale({ 20, 20, 20 });

	//3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup);


	//当たり判定 球 の初期値を設定
	sphere.center = XMVectorSet(0, 2, 0, 1); //中心点座標
	sphere.radius = 1.0f; //半径

	//当たり判定 平面 の初期値を設定
	plane.normal = XMVectorSet(0, 1, 0, 0); //法線ベクトル
	plane.distance = 0.0f; //原点(0, 0, 0)からの距離

	//当たり判定 三角形 の初期値を設定
	triangle.p0 = XMVectorSet(-1.0f, +1.0, -1.0f, 1);	//左手前
	triangle.p1 = XMVectorSet(-1.0f, +1.0, +1.0f, 1);	//左奥
	triangle.p2 = XMVectorSet(+1.0f, +1.0, -1.0f, 1);	//右手前
	triangle.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0);	//上向き

	//当たり判定 レイ の初期値を設定
	ray.start = XMVectorSet(0, 5, 0, 1);	//原点やや上
	ray.dir = XMVectorSet(0, -1, 0, 0);		//下向き
}

void TurretScene::Finalize()
{
	//カメラ解放
	delete camera;

	//ライト解放
	delete lightGroup;

	//パーティクル解放
	delete particleMan;

	//スプライト解放
	delete sprite;
	delete sprite2;

	//モデル解放
	delete modelSkydome;
	delete modelGround;
	delete modelSphere;
	delete modelFighter;

	//3Dオブジェクト解放
	delete objGround;
	delete objSkydome;
	delete objTurret;
}

void TurretScene::Update()
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
	//	lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	//	lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	//	lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
	//	lightGroup->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0], spotLightDir[1], spotLightDir[2], 0 }));
	//	lightGroup->SetSpotLightPos(0, XMFLOAT3(spotLightPos));
	//	lightGroup->SetSpotLightColor(0, XMFLOAT3(spotLightColor));
	//	lightGroup->SetSpotLightAtten(0, XMFLOAT3(spotLightAtten));
	//	lightGroup->SetSpotLightFactorAngleCos(0, XMFLOAT2(spotLightfactorAngleCos));
		lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
		lightGroup->SetCircleShadowCasterPos(0, objTurret->GetBulletPosition());
		lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngleCos(0, XMFLOAT2(circleShadowFactorAngle));
	}

	//ライト更新
	lightGroup->Update();


	//Object3d更新
	objGround->Update();
	objSkydome->Update();
	objTurret->Update();

	XMFLOAT3 eye = objTurret->GetBulletPosition();
	eye.z -= 200;
	camera->SetEye(eye);
	camera->SetTarget(objTurret->GetBulletPosition());

	//カメラ更新
	camera->Update();

	//デバックテキスト
	//X座標,Y座標,縮尺を指定して表示
	debugText->Print("TurretShot", 100, 50);
	debugText->Print("R key Reset", 100, 100);
	debugText->Print("ENTER FallSphereScene", 100, 130);


	//エンターキーで重力落下シーンへ
	if(input->TriggerKey(DIK_RETURN))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TurretScene::Draw()
{
	//Object3d共通コマンド
	Object3d::DrawPrev();
	///-------Object3d描画ここから-------///


	objGround->Draw();
	objSkydome->Draw();
	objTurret->Draw();


	///-------Object3d描画ここまで-------///


	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///


	//sprite->Draw();
	//sprite2->Draw();	


	///-------スプライト描画ここまで-------///


	//パーティクル共通コマンド
	ParticleManager::DrawPrev();
	///-------パーティクル描画ここから-------///


	//パーティクル描画
	//particleMan->Draw();


	///-------パーティクル描画ここまで-------///
}
