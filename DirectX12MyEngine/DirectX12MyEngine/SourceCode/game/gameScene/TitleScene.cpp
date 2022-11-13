#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "SceneChangeEffect.h"
#include <cassert>
#include <fstream>
#include <iomanip>

void TitleScene::Initialize()
{
	//ライト生成
	lightGroup.reset(LightGroup::Create());

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
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydome"));
	modelGround.reset(ObjModel::LoadFromOBJ("ground"));
	modelMountain.reset(ObjModel::LoadFromOBJ("mountain"));
	modelSphere.reset(ObjModel::LoadFromOBJ("sphere", true));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter"));

	//自機生成
	player.reset(TitlePlayer::Create(modelFighter.get(), { 0, 5, 0 }));

	//カメラ初期化
	titleCamera.reset(new TitleCamera());
	titleCamera->SetTitlePlayer(player.get());
	titleCamera->Initialize();

	//天球生成
	skydome.reset(Skydome::Create(modelSkydome.get()));

	//地面生成
	ground.reset(Ground::Create(modelGround.get()));

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(titleCamera.get());
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

	//オブジェクト更新
	//自機
	player->Update();
	//天球
	skydome->Update();
	//地面
	ground->Update();
	//背景用(山)
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->Update();
	}

	//スプライト更新
	//タイトルUI更新
	titleUI->Update();
	

	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();

	//デバックテキスト
	//X座標,Y座標,縮尺を指定して表示
	//debugText->Print("GAME SCENE", 1000, 50);

	//自機が空まで行って見えなくなったら
	const bool isPlayerSky = (player->GetPosition().y >= 500);
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
}

void TitleScene::DrawBackSprite() 
{
}

void TitleScene::Draw3D()
{
	//Object3d共通コマンド
	ObjObject3d::DrawPrev();
	///-------Object3d描画ここから-------///

	//自機
	player->Draw();
	//天球
	skydome->Draw();
	//地面
	ground->Draw();
	//背景用(山)
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->Draw();
	}

	///-------Object3d描画ここまで-------///


	//パーティクル共通コマンド
	ParticleManager::DrawPrev();
	///-------パーティクル描画ここから-------///

	//パーティクル描画
	ParticleEmitter::GetInstance()->DrawAll();

	///-------パーティクル描画ここまで-------///
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
