#include "SortieScene.h"
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

void SortieScene::Initialize()
{
	//ライト生成
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	//objからモデルデータを読み込む
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydome"));
	modelGround.reset(ObjModel::LoadFromOBJ("ground"));
	modelMountain.reset(ObjModel::LoadFromOBJ("mountain"));
	modelSphere.reset(ObjModel::LoadFromOBJ("sphere", true));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter"));

	//自機生成
	player.reset(SortiePlayer::Create(modelFighter.get(), { 0, 20, -1000 }));

	//カメラ初期化
	sortieCamera.reset(new SortieCamera());
	sortieCamera->SetSortiePlayer(player.get());
	sortieCamera->Initialize();
	//影用光源カメラ初期化
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ 0, 500, 0 });
	lightCamera->SetProjectionNum({ 100, 100 }, { -100, -100 });
	//頭上からの影用光源カメラ初期化
	topLightCamera.reset(new LightCamera());
	topLightCamera->Initialize({ 0, 500, 0 });
	topLightCamera->SetProjectionNum({ 100, 100 }, { -100, -100 });

	//天球生成
	skydome.reset(Skydome::Create(modelSkydome.get()));
	//見栄えがいい角度に変更しておく
	skydome->SetRotation({ 0, 180, 0 });

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
	ObjObject3d::SetLightCamera(lightCamera.get());
	ObjObject3d::SetTopLightCamera(topLightCamera.get());
	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup.get());

	//パーティクルにカメラをセット
	ParticleManager::SetCamera(sortieCamera.get());
	//画面にパーティクルが残ることがあるので全て削除しておく
	ParticleEmitter::GetInstance()->AllDelete();

	//ポストエフェクトのブラーをかける
	GamePostEffect::GetPostEffect()->SetRadialBlur(true);
	const float blurStrength = 0.2f;
	GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
}

void SortieScene::Update()
{
	//デバッグテキストのインスタンスを取得
	DebugText* debugText = DebugText::GetInstance();

	//出撃挙動管理
	SortieAction();

	//カメラ更新
	sortieCamera->Update();
	//影生成用ライトカメラ
	LightCameraUpdate();

	//ライト更新
	lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
	lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2], 0 }));
	lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));
	lightGroup->Update();

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


	//自機の出撃行動が完了したら
	if (player->GetIsSortieEnd()) {
		//ゲームシーンへシーン変更を開始する
		SceneChangeStart({ 1,1,1,0 }, 10, 0, 20, "GAME");
	}
	//ムービースキップ機能
	MovieSkip();

	//シーン変更状態
	SceneChangeMode();

	//シーン変更演出更新
	SceneChangeEffect::Update();
}

void SortieScene::DrawBackSprite()
{
}

void SortieScene::Draw3D()
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

	///-------パーティクル描画ここから-------///

	//パーティクル描画
	ParticleEmitter::GetInstance()->DrawAll();

	///-------パーティクル描画ここまで-------///
}

void SortieScene::Draw3DLightView()
{
	//Object3d共通コマンド
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d描画ここから-------///

	//背景用(山)
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->DrawLightCameraView();
	}

	///-------Object3d描画ここまで-------///
}

void SortieScene::Draw3DTopLightView()
{
	//Object3d共通コマンド
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d描画ここから-------///

	//自機
	player->DrawTopLightCameraView();

	///-------Object3d描画ここまで-------///
}

void SortieScene::DrawFrontSprite()
{
	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///

	//シーン変更演出描画
	SceneChangeEffect::Draw();


	///-------スプライト描画ここまで-------///
}

void SortieScene::LightCameraUpdate()
{
	//ターゲットになる座標
	const Vector3 targetPos = sortieCamera->GetEye();
	//ターゲットと視点の距離
	const Vector3 targetDistance = { -300, 200, -300 };
	//ライトカメラ用の視点
	const Vector3 lightEye = targetPos + targetDistance;
	lightCamera->SetEyeTarget(lightEye, targetPos);
	lightCamera->Update();


	//頭上からのライトカメラ用ターゲットと視点の距離
	const Vector3 topCameraTargetDistance = { 0, 500, 350 };
	//頭上からのライトカメラ用の視点
	const Vector3 topLightEye = targetPos + topCameraTargetDistance;
	topLightCamera->SetEyeTarget(topLightEye, targetPos);
	topLightCamera->Update();
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

void SortieScene::MovieSkip()
{
	//スキップ入力がなければ抜ける
	if (!(Input::GetInstance()->TriggerKey(DIK_RETURN) || Input::GetInstance()->TriggerGamePadButton(Input::PAD_START))) { return; }

	//ゲームシーンへシーン変更を開始する
	SceneChangeStart({ 0,0,0,0 }, 15, 0, 15, "GAME");
}
