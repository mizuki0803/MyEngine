#include "StageSelectScene.h"
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

StageSelectScene::SelectStage StageSelectScene::selectStage = StageSelectScene::SelectStage::Title;

void StageSelectScene::Initialize()
{
	//ライト生成
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	//objからモデルデータを読み込む
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeSpace"));
	modelGround.reset(ObjModel::LoadFromOBJ("ground"));
	modelMountain.reset(ObjModel::LoadFromOBJ("mountain"));
	modelSphere.reset(ObjModel::LoadFromOBJ("sphere", true));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter"));
	modelStageSelect.reset(ObjModel::LoadFromOBJ("stageSelect", true));
	modelPlanetBasis.reset(ObjModel::LoadFromOBJ("planetBasis", true));
	modelPlanetMautan.reset(ObjModel::LoadFromOBJ("planetMautan", true));
	modelPlanetSoon.reset(ObjModel::LoadFromOBJ("planetSoon", true));

	//ポストエフェクトのブラーを解除しておく
	GamePostEffect::GetPostEffect()->SetRadialBlur(false);

	//ステージ選択フィールド生成
	stageSelectField.reset(ObjObject3d::Create(modelStageSelect.get()));
	stageSelectField->SetScale({ 5, 5, 5 });
	stageSelectField->SetIsShadowMap(true);

	//惑星生成
	CreatePlanets();

	//自機生成
	player.reset(StageSelectPlayer::Create(modelFighter.get(), planets[(int)selectStage]->GetPosition()));
	player->GooutPlanetStart(stageSelectFieldPos[(int)selectStage]);

	//カメラ初期化
	stageSelectCamera.reset(new StageSelectCamera());
	stageSelectCamera->SetStageSelectPlayer(player.get());
	stageSelectCamera->Initialize();
	stageSelectCamera->GooutPlayerLookActionStart(planets[(int)selectStage]->GetPosition(), stageSelectFieldPos[(int)selectStage]);

	//影用光源カメラ初期化
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ 0, -500, 0 });
	lightCamera->SetProjectionNum({ 30, 30 }, { -30, -30 });
	//頭上からの影用光源カメラ初期化
	topLightCamera.reset(new LightCamera());
	topLightCamera->Initialize({ 0, 500, 0 });
	topLightCamera->SetProjectionNum({ 30, 30 }, { -30, -30 });

	//天球生成
	skydome.reset(Skydome::Create(modelSkydome.get()));
	skydome->SetIsShadowMap(true);

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(stageSelectCamera.get());
	ObjObject3d::SetLightCamera(lightCamera.get());
	ObjObject3d::SetTopLightCamera(topLightCamera.get());
	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup.get());

	//ステージ選択シーンUI
	stageSelectUI.reset(StageSelectUI::Create());
	//ステージ決定確認UI生成
	stageDecisionUI.reset(StageDecisionUI::Create());

	//パーティクルにカメラをセット
	ParticleManager::SetCamera(stageSelectCamera.get());
	//画面にパーティクルが残ることがあるので全て削除しておく
	ParticleEmitter::GetInstance()->AllDelete();
}

void StageSelectScene::Update()
{
	//デバッグテキストのインスタンスを取得
	DebugText* debugText = DebugText::GetInstance();

	//惑星から出てくる行動
	GooutPlanet();
	//ステージ選択
	StageSelect();
	//ステージ決定
	StageDicision();
	//惑星に入る行動
	EnterPlanet();

	//カメラ更新
	stageSelectCamera->Update();
	//影生成用ライトカメラ
	LightCameraUpdate();

	//ライト更新
	{
		lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
		lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2], 0 }));
		lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));
		lightGroup->SetDirLightDir(1, XMVECTOR({ lightDir1[0], lightDir1[1], lightDir1[2], 0 }));
		lightGroup->SetDirLightColor(1, XMFLOAT3(lightColor1));
		lightGroup->SetDirLightDir(2, XMVECTOR({ lightDir2[0], lightDir2[1], lightDir2[2], 0 }));
		lightGroup->SetDirLightColor(2, XMFLOAT3(lightColor2));
	}
	lightGroup->Update();

	//オブジェクト更新
	//自機
	player->Update();
	//惑星
	for (const std::unique_ptr<StageSelectPlanet>& planet : planets) {
		planet->Update();
	}
	//天球
	Vector3 rot = skydome->GetRotation();
	rot.y += 0.04f;
	skydome->SetRotation(rot);
	skydome->Update();
	//ステージ選択フィールド
	stageSelectField->Update();

	//スプライト更新
	//ステージ選択シーンUI
	stageSelectUI->Update();
	//ステージ決定確認UI
	stageDecisionUI->Update();

	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();

	//デバッグ用シーン切り替え
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAME");
	}

	//シーン変更状態
	SceneChangeMode();

	//シーン変更演出更新
	SceneChangeEffect::Update();
}

void StageSelectScene::DrawBackSprite()
{
}

void StageSelectScene::Draw3D()
{
	//Object3d共通コマンド
	ObjObject3d::DrawPrev();
	///-------Object3d描画ここから-------///

	//自機
	player->Draw();
	//惑星
	for (const std::unique_ptr<StageSelectPlanet>& planet : planets) {
		planet->Draw();
	}
	//天球
	skydome->Draw();
	//ステージ選択フィールド
	stageSelectField->Draw();

	///-------Object3d描画ここまで-------///

	///-------パーティクル描画ここから-------///

	//パーティクル描画
	ParticleEmitter::GetInstance()->DrawAll();

	///-------パーティクル描画ここまで-------///
}

void StageSelectScene::Draw3DLightView()
{
	//Object3d共通コマンド
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d描画ここから-------///

	//惑星
	for (const std::unique_ptr<StageSelectPlanet>& planet : planets) {
		planet->DrawLightCameraView();
	}
	//天球
	skydome->DrawLightCameraView();
	//ステージ選択フィールド
	stageSelectField->DrawLightCameraView();

	///-------Object3d描画ここまで-------///
}

void StageSelectScene::Draw3DTopLightView()
{
	//Object3d共通コマンド
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d描画ここから-------///

	//自機
	player->DrawTopLightCameraView();

	///-------Object3d描画ここまで-------///
}

void StageSelectScene::DrawFrontSprite()
{
	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///

	//ステージ選択シーンUI
	stageSelectUI->Draw();
	//ステージ決定確認UI
	stageDecisionUI->Draw();

	//シーン変更演出描画
	SceneChangeEffect::Draw();


	///-------スプライト描画ここまで-------///
}

void StageSelectScene::LightCameraUpdate()
{
	//ターゲットになる座標
	const Vector3 targetPos = player->GetPosition();
	//ターゲットと視点の距離
	const Vector3 targetDistance = { 0, 500, -100 };
	//ライトカメラ用の視点
	const Vector3 lightEye = targetPos + targetDistance;
	topLightCamera->SetEyeTarget(lightEye, targetPos);
	topLightCamera->Update();
}

void StageSelectScene::CreatePlanets()
{
	//ステージ選択フィールド座標をセット
	const float fieldDistance = 20.0f;
	for (int i = 0; i < 3; i++) {
		Vector3 pos = { stageSelectField->GetScale().x * fieldDistance * i, 0, 0 };
		stageSelectFieldPos.push_back(pos);
	}

	//惑星とステージ選択フィールドの距離
	const Vector3 planetFieldDistance = { 0, 0, 50 };

	//タイトル用惑星生成
	std::unique_ptr<StageSelectPlanet> titlePlanet;
	titlePlanet.reset(StageSelectPlanet::Create(modelPlanetBasis.get(), stageSelectFieldPos[0] + planetFieldDistance));
	planets.push_back(std::move(titlePlanet));

	//ステージ01用惑星生成
	std::unique_ptr<StageSelectPlanet> stage01Planet;
	stage01Planet.reset(StageSelectPlanet::Create(modelPlanetMautan.get(), stageSelectFieldPos[1] + planetFieldDistance));
	planets.push_back(std::move(stage01Planet));

	//まだないステージ用惑星生成
	std::unique_ptr<StageSelectPlanet> comingSoonPlanet;
	comingSoonPlanet.reset(StageSelectPlanet::Create(modelPlanetSoon.get(), stageSelectFieldPos[2] + planetFieldDistance));
	planets.push_back(std::move(comingSoonPlanet));
}

void StageSelectScene::GooutPlanet()
{
	//カメラが惑星から出てくる自機を見る行動状態でなければ抜ける
	if (!(stageSelectCamera->GetActionPhase() == StageSelectCamera::ActionPhase::GooutPlayerLook)) { return; }

	//自機の惑星から出てくる移動が終わったら
	if (!player->GetIsGooutPlanetEnd()) { return; }

	//カメラのステージ選択開始状態を開始する
	stageSelectCamera->StageSelectStartActionStart();
	//ステージ選択シーンUIを画面内に表示させる
	stageSelectUI->TextInScreenStart((int)selectStage);
}

void StageSelectScene::StageSelect()
{
	//ステージ選択確認中なら抜ける
	if (isStageDecisionCheck) { return; }
	//自機がステージ選択状態でなければ抜ける
	if (!(player->GetActionPhase() == StageSelectPlayer::ActionPhase::StageSelect)) { return; }
	//カメラがステージ選択状態でなければ抜ける
	if (!(stageSelectCamera->GetActionPhase() == StageSelectCamera::ActionPhase::StageSelect)) { return; }
	//ステージ選択で自機が移動中なら抜ける
	if (player->GetIsStageSelectMove()) { return; }

	//自機の移動が完了してステージ選択シーンUIを表示していない場合は表示を開始する
	if (!stageSelectUI->GetIsShow()) { stageSelectUI->TextInScreenStart((int)selectStage); }

	//入力のインスタンスを取得
	Input* input = Input::GetInstance();

	//スティックをどこまで傾けたら判定をとるか
	const float stickNum = 200;
	//右か左の入力がなければ抜ける
	const bool isInputRight = (input->PushKey(DIK_D) || input->TiltGamePadLStickX(stickNum) || input->PushGamePadButton(Input::PAD_RIGHT));
	const bool isInputLeft = (input->PushKey(DIK_A) || input->TiltGamePadLStickX(-stickNum) || input->PushGamePadButton(Input::PAD_LEFT));
	if (!(isInputRight || isInputLeft)) { return; }

	//右方向に入力していたら選択するステージを右に変更
	if (isInputRight) { StageSelectChangeRight(); }
	//左方向に入力していたら選択するステージを左に変更
	else if (isInputLeft) { StageSelectChangeLeft(); }
}

void StageSelectScene::StageSelectChangeRight()
{
	//移動できたか
	bool isMove = false;

	//選択するステージ番号を1つ進める
	if (selectStage == SelectStage::Title) { selectStage = SelectStage::Stage01; isMove = true; }
	else if (selectStage == SelectStage::Stage01) { selectStage = SelectStage::ComingSoon; isMove = true; }

	//移動できなければ抜ける
	if (!isMove) { return; }

	//自機をステージ選択移動させる
	player->StageSelectMoveStart(stageSelectFieldPos[(int)selectStage], true);

	//ステージ選択シーンUIを画面外に移動させる
	stageSelectUI->TextOutScreenStart();
}

void StageSelectScene::StageSelectChangeLeft()
{
	//移動できたか
	bool isMove = false;

	//選択するステージ番号を1つ戻す
	if (selectStage == SelectStage::ComingSoon) { selectStage = SelectStage::Stage01; isMove = true; }
	else if (selectStage == SelectStage::Stage01) { selectStage = SelectStage::Title; isMove = true; }

	//移動できなければ抜ける
	if (!isMove) { return; }

	//自機をステージ選択移動させる
	player->StageSelectMoveStart(stageSelectFieldPos[(int)selectStage], false);

	//ステージ選択シーンUIを画面外に移動させる
	stageSelectUI->TextOutScreenStart();
}

void StageSelectScene::StageDicision()
{
	//入力のインスタンスを取得
	Input* input = Input::GetInstance();

	//ステージ決定確認中でなければ
	if (!isStageDecisionCheck) {
		//選しているステージが制作中なら抜ける
		if (selectStage == SelectStage::ComingSoon) { return; }

		//自機がステージ選択状態でなければ抜ける
		if (!(player->GetActionPhase() == StageSelectPlayer::ActionPhase::StageSelect)) { return; }
		//カメラがステージ選択状態でなければ抜ける
		if (!(stageSelectCamera->GetActionPhase() == StageSelectCamera::ActionPhase::StageSelect)) { return; }
		//ステージ選択で自機が移動中なら抜ける
		if (player->GetIsStageSelectMove()) { return; }
		//決定入力がなければ抜ける
		if (!(input->TriggerKey(DIK_SPACE) || input->TriggerGamePadButton(Input::PAD_B))) { return; }

		//ステージ決定確認中にする
		isStageDecisionCheck = true;

		//カメラをステージ決定確認状態にする
		stageSelectCamera->StageDecisionCheckActionStart();
		//ステージ選択シーンUIを画面外に移動させる
		stageSelectUI->TextOutScreenStart();
		//ステージ決定確認UIを画面に出す
		stageDecisionUI->TextUpStart((int)selectStage);
	}
	//ステージ決定確認中なら
	else {
		//ステージ決定確認UIが表示状態でなければ抜ける
		if (!(stageDecisionUI->GetActionPhase() == StageDecisionUI::ActionPhase::Show)) { return; }

		//ステージ決定するか選択
		SelectStageDicision();

		//決定入力がなければ抜ける
		if (!(input->TriggerKey(DIK_SPACE) || input->TriggerGamePadButton(Input::PAD_B) || input->TriggerGamePadButton(Input::PAD_A))) { return; }

		//A入力は確定でステージに入らない
		if (input->TriggerGamePadButton(Input::PAD_A)) {

			//ステージ決定確認中を解除
			isStageDecisionCheck = false;
			//ステージ決定しない状態にしておく
			stageDecisionUI->ChangeStageDecision(false);
			//ステージ決定確認UIを画面外に戻す
			stageDecisionUI->TextDownStart();
		}

		//ステージ決定確認中を解除
		isStageDecisionCheck = false;

		//ステージ決定確認UIを画面外に戻す
		stageDecisionUI->TextDownStart();

		//ステージ決定なら自機とカメラを惑星に入る行動に切り替える
		if (stageDecisionUI->GetIsStageDecision()) {
			player->EnterPlanetStart(planets[(int)selectStage]->GetPosition());
		}
		//ステージ決定しないなら
		else {
			//カメラをステージ選択状態に戻す
			stageSelectCamera->ReturnSelectActionStart();
			//ステージ選択シーンUIを画面内に表示させる
			stageSelectUI->TextInScreenStart((int)selectStage);
		}
	}
}

void StageSelectScene::SelectStageDicision()
{
	//入力のインスタンスを取得
	Input* input = Input::GetInstance();

	//スティックをどこまで傾けたら判定をとるか
	const float stickNum = 200;
	//右か左の入力がなければ抜ける
	const bool isInputUp = (input->PushKey(DIK_W) || input->TiltGamePadLStickY(-stickNum) || input->PushGamePadButton(Input::PAD_UP));
	const bool isInputDown = (input->PushKey(DIK_S) || input->TiltGamePadLStickY(stickNum) || input->PushGamePadButton(Input::PAD_DOWN));
	const bool isInputA = (input->TriggerGamePadButton(DIK_A));
	if (!(isInputUp || isInputDown)) { return; }

	//入力で切り替える
	if (isInputUp) { stageDecisionUI->ChangeStageDecision(true); }
	else if (isInputDown) { stageDecisionUI->ChangeStageDecision(false); }
}

void StageSelectScene::EnterPlanet()
{
	//自機が惑星に入る状態でなければ抜ける
	if (!(player->GetActionPhase() == StageSelectPlayer::ActionPhase::EnterPlanet)) { return; }

	//自機がステージ選択で行う全ての行動を終えていなければ抜ける
	if (!player->GetIsStageSelectModeEnd()) { return; }

	//シーンを切り替える
	ChangeSelectScene();
}

void StageSelectScene::ChangeSelectScene()
{
	//選択しているステージに応じてシーン変更
	if (selectStage == SelectStage::Title) { SceneChangeStart({ 0,0,0,0 }, 40, 60, 60, "TITLE"); }
	else if (selectStage == SelectStage::Stage01) { SceneChangeStart({ 0,0,0,0 }, 40, 60, 120, "SORTIE"); }
}
