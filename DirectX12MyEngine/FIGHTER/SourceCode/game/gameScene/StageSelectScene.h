#pragma once
#include "BaseGameScene.h"
#include "ObjObject3d.h"
#include "LightGroup.h"
#include "StageSelectCamera.h"
#include "LightCamera.h"
#include "StageSelectPlayer.h"
#include "StageSelectPlanet.h"
#include "Skydome.h"
#include "Ground.h"
#include "StageSelectUI.h"
#include "StageDecisionUI.h"

#include <sstream>

/// <summary>
/// ステージ選択シーン
/// </summary>
class StageSelectScene :public BaseGameScene
{
public:
	//選択するステージ
	enum class SelectStage
	{
		Title,		//タイトルシーン
		Stage01,	//ステージ01シーン
		ComingSoon,	//まだないステージ
	};

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 背景スプライト描画
	/// </summary>
	void DrawBackSprite() override;

	/// <summary>
	/// 3Dシーン描画
	/// </summary>
	void Draw3D() override;

	/// <summary>
	/// 3Dシーン描画(ライトから見た視点)
	/// </summary>
	void Draw3DLightView() override;

	/// <summary>
	/// 3Dシーン描画(頭上からのライトから見た視点)
	/// </summary>
	void Draw3DTopLightView() override;

	/// <summary>
	/// 前景スプライト描画
	/// </summary>
	void DrawFrontSprite() override;

private: //メンバ関数
	/// <summary>
	/// 影生成用ライトカメラ更新
	/// </summary>
	void LightCameraUpdate();

	/// <summary>
	/// 惑星生成
	/// </summary>
	void CreatePlanets();

	/// <summary>
	/// 惑星から出てくる
	/// </summary>
	/// <returns></returns>
	void GooutPlanet();

	/// <summary>
	/// ステージ選択
	/// </summary>
	void StageSelect();

	/// <summary>
	/// 選択するステージを右に変更
	/// </summary>
	void StageSelectChangeRight();

	/// <summary>
	/// 選択するステージを左に変更
	/// </summary>
	void StageSelectChangeLeft();

	/// <summary>
	/// ステージ決定
	/// </summary>
	void StageDicision();

	/// <summary>
	/// ステージ決定するか選択
	/// </summary>
	void SelectStageDicision();

	/// <summary>
	/// 惑星に入る行動
	/// </summary>
	void EnterPlanet();

	/// <summary>
	/// 選択したシーンに変更
	/// </summary>
	void ChangeSelectScene();


private: //静的メンバ変数
	//選択しているステージ
	static SelectStage selectStage;

private: //メンバ変数
	//カメラ
	std::unique_ptr<StageSelectCamera> stageSelectCamera;
	//影用光源カメラ
	std::unique_ptr<LightCamera> lightCamera;
	//頭上からの影用光源カメラ
	std::unique_ptr<LightCamera> topLightCamera;

	//ライト
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// 光線方向初期値
	float lightDir0[3] = { 0,-3,-4 };
	float lightColor0[3] = { 1,1,1 };
	float lightDir1[3] = { 1,-2,4 };
	float lightColor1[3] = { 1,1,1 };
	float lightDir2[3] = { -1,-2,4 };
	float lightColor2[3] = { 1,1,1 };

	//objモデルデータ
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelFighter;
	std::unique_ptr<ObjModel> modelStageSelect;
	std::unique_ptr<ObjModel> modelPlanetBasis;
	std::unique_ptr<ObjModel> modelPlanetTrial;
	std::unique_ptr<ObjModel> modelPlanetSoon;

	//ステージ選択用惑星
	std::vector<std::unique_ptr<StageSelectPlanet>> planets;
	//ステージ選択フィールド座標
	std::vector<Vector3> stageSelectFieldPos;
	//ステージ選択用自機
	std::unique_ptr<StageSelectPlayer> player;
	//天球
	std::unique_ptr<Skydome> skydome;
	//地面
	std::unique_ptr<Ground> ground;
	//ステージ選択フィールド
	std::unique_ptr<ObjObject3d> stageSelectField;

	//ステージ選択シーンUI
	std::unique_ptr<StageSelectUI> stageSelectUI;
	//ステージ決定確認UI
	std::unique_ptr<StageDecisionUI> stageDecisionUI;

	//ステージ決定確認中か
	bool isStageDecisionCheck = false;
};