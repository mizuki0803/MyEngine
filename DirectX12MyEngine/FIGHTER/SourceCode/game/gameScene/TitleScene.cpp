#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"
#include <cassert>
#include <fstream>
#include <iomanip>

void TitleScene::Initialize()
{
	//ライト生成
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, false);

	//objからモデルデータを読み込む
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeBase"));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter"));
	modelbaseField.reset(ObjModel::LoadFromOBJ("baseField"));
	modelbaseField02.reset(ObjModel::LoadFromOBJ("baseField02"));
	modelRoad.reset(ObjModel::LoadFromOBJ("road"));
	modelWarehouse01.reset(ObjModel::LoadFromOBJ("warehouse01"));
	modelWarehouse02.reset(ObjModel::LoadFromOBJ("warehouse02", true));
	modelControlTower.reset(ObjModel::LoadFromOBJ("controlTower", true));
	modelControlRoom.reset(ObjModel::LoadFromOBJ("controlRoom", true));

	//背景マップレベルデータ生成
	backgroundMapData.reset(LevelDataLoader::Create("titleMap.json"));
	backgroundMapData->InsertModel("fighter", modelFighter.get());
	backgroundMapData->InsertModel("baseField", modelbaseField.get());
	backgroundMapData->InsertModel("baseField02", modelbaseField02.get());
	backgroundMapData->InsertModel("road", modelRoad.get());
	backgroundMapData->InsertModel("controlTower", modelControlTower.get());
	backgroundMapData->InsertModel("controlRoom", modelControlRoom.get());
	backgroundMapData->InsertModel("warehouse01", modelWarehouse01.get());
	backgroundMapData->InsertModel("warehouse02", modelWarehouse02.get());
	backgroundMapData->CreateLevelDataObjects();

	//ポストエフェクトのブラーを解除しておく
	GamePostEffect::GetPostEffect()->SetRadialBlur(false);

	//自機生成
	player.reset(TitlePlayer::Create(modelFighter.get(), { 0, 2, 0 }));

	//カメラ初期化
	titleCamera.reset(new TitleCamera());
	titleCamera->SetTitlePlayer(player.get());
	titleCamera->Initialize();
	//影用光源カメラ初期化
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ -100, 100, -300 });
	lightCamera->SetProjectionNum({ 400, 400 }, { -400, -400 });
	//頭上からの影用光源カメラ初期化
	topLightCamera.reset(new LightCamera());
	topLightCamera->Initialize({ 0, 300, 10 });
	topLightCamera->SetProjectionNum({ 100, 100 }, { -100, -100 });

	//天球生成
	skydome.reset(Skydome::Create(modelSkydome.get()));

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(titleCamera.get());
	ObjObject3d::SetLightCamera(lightCamera.get());
	ObjObject3d::SetTopLightCamera(topLightCamera.get());
	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup.get());

	//パーティクルにカメラをセット
	ParticleManager::SetCamera(titleCamera.get());
	//画面にパーティクルが残ることがあるので全て削除しておく
	ParticleEmitter::GetInstance()->AllDelete();

	//タイトルUI生成
	titleUI.reset(TitleUI::Create());
}

void TitleScene::Update()
{
	//入力のインスタンスを取得
	Input* input = Input::GetInstance();
	//デバッグテキストのインスタンスを取得
	DebugText* debugText = DebugText::GetInstance();

	//出撃開始
	SortieStart();


	//カメラ更新
	titleCamera->Update();
	lightCamera->Update();
	topLightCamera->Update();

	//ライト更新
	lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
	lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2], 0 }));
	lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));
	lightGroup->SetDirLightDir(1, XMVECTOR({ lightDir1[0], lightDir1[1], lightDir1[2], 0 }));
	lightGroup->SetDirLightColor(1, XMFLOAT3(lightColor1));
	lightGroup->Update();

	//オブジェクト更新
	//背景マップレベルデータ
	backgroundMapData->Update();
	//自機
	player->Update();
	//天球
	skydome->Update();

	//スプライト更新
	//タイトルUI更新
	titleUI->Update();


	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();

	//自機が空まで行って見えなくなったら
	const float stageChangePlayerPos = 550;
	const bool isPlayerSky = (player->GetPosition().y >= stageChangePlayerPos);
	if (isPlayerSky) {
		//ステージ選択へシーン変更を開始する
		SceneChangeStart({ 0,0,0,0 }, 40, 60, 60, "STAGESELECT");
	}
	//ムービースキップ機能
	MovieSkip();

	//シーン変更状態
	SceneChangeMode();

	//シーン変更演出更新
	SceneChangeEffect::Update();

	if (input->TriggerKey(DIK_Q)) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
	if (input->TriggerKey(DIK_W)) {
		SceneManager::GetInstance()->ChangeScene("SORTIE");
	}
}

void TitleScene::DrawBackSprite()
{
}

void TitleScene::Draw3D()
{
	//Object3d共通コマンド
	ObjObject3d::DrawPrev();
	///-------Object3d描画ここから-------///

	//背景マップレベルデータ
	backgroundMapData->Draw();
	//自機
	player->Draw();
	//天球
	skydome->Draw();

	///-------Object3d描画ここまで-------///

	///-------パーティクル描画ここから-------///

	//パーティクル描画
	ParticleEmitter::GetInstance()->DrawAll();

	///-------パーティクル描画ここまで-------///
}

void TitleScene::Draw3DLightView()
{
	//Object3d共通コマンド
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d描画ここから-------///

	//背景マップレベルデータ
	backgroundMapData->DrawLightCameraView();

	///-------Object3d描画ここまで-------///
}

void TitleScene::Draw3DTopLightView()
{
	//Object3d共通コマンド
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d描画ここから-------///

	//自機
	player->DrawTopLightCameraView();

	///-------Object3d描画ここまで-------///
}

void TitleScene::DrawFrontSprite()
{
	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///

	//タイトルUI
	if (titleUI) {
		titleUI->Draw();
	}

	//シーン変更演出描画
	SceneChangeEffect::Draw();

	///-------スプライト描画ここまで-------///
}

void TitleScene::SortieStart()
{
	//既に出撃していたら抜ける
	if (player->GetIsSortie()) { return; }
	//スペースキーを押していなければ抜ける
	if (!(Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->TriggerGamePadButton(Input::PAD_B))) { return; }

	//自機とカメラを出撃状態にする
	player->SetIsSortie(true);
	titleCamera->SortieStart();

	//タイトルUIの表示を終える
	titleUI->SetShowEnd();
}

void TitleScene::MovieSkip()
{
	//出撃していなければ抜ける
	if (!player->GetIsSortie()) { return; }
	//スキップ入力がなければ抜ける
	if (!(Input::GetInstance()->TriggerKey(DIK_RETURN) || Input::GetInstance()->TriggerGamePadButton(Input::PAD_START))) { return; }

	//ステージ選択へシーン変更を開始する
	SceneChangeStart({ 0,0,0,0 }, 40, 60, 60, "STAGESELECT");
}
