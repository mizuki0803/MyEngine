#pragma once
#include "BaseGameScene.h"
#include "Sprite.h"
#include "ObjObject3d.h"
#include "LightGroup.h"
#include "TitleCamera.h"
#include "LightCamera.h"
#include "CollisionShape.h"
#include "Collision.h"
#include "TitlePlayer.h"
#include "Skydome.h"
#include "Ground.h"
#include "Mountain.h"
#include "TitleUI.h"
#include "LevelDataLoader.h"

#include <sstream>

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene :public BaseGameScene
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

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
	/// 前景スプライト描画
	/// </summary>
	void DrawFrontSprite() override;

private: //メンバ関数
	/// <summary>
	/// 出撃開始
	/// </summary>
	void SortieStart();

	/// <summary>
	/// ムービースキップ機能
	/// </summary>
	void MovieSkip();

private: //メンバ変数
	//カメラ
	std::unique_ptr<TitleCamera> titleCamera;
	//影用光源カメラ
	std::unique_ptr<LightCamera> lightCamera;

	//ライト
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// 光線方向初期値
	float lightDir0[3] = { 1,-1,-12 };
	float lightColor0[3] = { 1,1,1 };

	//objモデルデータ
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelFighter;
	std::unique_ptr<ObjModel> modelbaseField;
	std::unique_ptr<ObjModel> modelRoad;
	std::unique_ptr<ObjModel> modelWarehouse01;
	std::unique_ptr<ObjModel> modelWarehouse02;
	std::unique_ptr<ObjModel> modelControlTower;

	//背景マップレベルデータ
	std::unique_ptr<LevelDataLoader> backgroundMapData;

	//タイトルシーン用自機
	std::unique_ptr<TitlePlayer> player;
	//天球
	std::unique_ptr<Skydome> skydome;

	//タイトルUI
	std::unique_ptr<TitleUI> titleUI;

	//その他変数
	float soundVol = 0.1f;
	char str[100];
};