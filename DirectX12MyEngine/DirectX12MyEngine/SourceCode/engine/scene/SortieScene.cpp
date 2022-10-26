#include "SortieScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include <cassert>
#include <fstream>
#include <iomanip>

void SortieScene::Initialize()
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
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter", true));

	//自機生成
	player.reset(SortiePlayer::Create(modelFighter.get(), { 0, 20, -1000 }));

	//カメラ初期化
	sortieCamera.reset(new SortieCamera());
	sortieCamera->SetSortiePlayer(player.get());
	sortieCamera->Initialize();

	//天球生成
	skydome.reset(Skydome::Create(modelSkydome.get()));

	//地面生成
	ground.reset(Ground::Create(modelGround.get()));

	//背景用(山)生成
	for (int i = 0; i < 40; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(modelMountain.get(), { -75, 0, -1000 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}
	for (int i = 0; i < 40; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(modelMountain.get(), { 75, 0, -1000 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(sortieCamera.get());
	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup.get());

	//パーティクルにカメラをセット
	ParticleManager::SetCamera(sortieCamera.get());
}

void SortieScene::Update()
{
	//入力のインスタンスを取得
	Input* input = Input::GetInstance();
	//デバッグテキストのインスタンスを取得
	DebugText* debugText = DebugText::GetInstance();

	//出撃挙動管理
	SortieAction();

	//カメラ更新
	sortieCamera->Update();

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

	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();

	//デバックテキスト
	//X座標,Y座標,縮尺を指定して表示
	//debugText->Print("GAME SCENE", 1000, 50);

	//自機の出撃行動が完了したら
	if (player->GetIsSortieEnd()) {
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
	if (input->TriggerKey(DIK_RETURN)) {
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void SortieScene::Draw()
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


	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///


	///-------スプライト描画ここまで-------///
}

void SortieScene::SortieAction()
{
	//既にブースト状態なら抜ける
	if (player->GetSortieActionPhase() == SortiePlayer::SortieActionPhase::Boost) { return; }
	//カメラの自機ズームが終了していなければ抜ける
	if (!sortieCamera->GetIsZoomEnd()) { return; }

	//自機をブースト状態にする
	player->BoostStart();
}
