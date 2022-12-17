#pragma once
#include "BaseGameScene.h"
#include "ObjObject3d.h"
#include "LightGroup.h"
#include "SortieCamera.h"
#include "LightCamera.h"
#include "SortiePlayer.h"
#include "Skydome.h"
#include "Ground.h"
#include "Mountain.h"

#include <sstream>

/// <summary>
/// 出撃演出シーン
/// </summary>
class SortieScene :public BaseGameScene
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
	/// 出撃
	/// </summary>
	void SortieAction();

	/// <summary>
	/// スキップ機能
	/// </summary>
	void MovieSkip();

private: //メンバ変数
	//カメラ
	std::unique_ptr<SortieCamera> sortieCamera;
	//影用光源カメラ
	std::unique_ptr<LightCamera> lightCamera;
	//頭上からの影用光源カメラ
	std::unique_ptr<LightCamera> topLightCamera;

	//ライト
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// 光線方向初期値
	float lightDir0[3] = { 0,-1,0 };
	float lightColor0[3] = { 1,1,1 };

	//objモデルデータ
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelGround;
	std::unique_ptr<ObjModel> modelMountain;
	std::unique_ptr<ObjModel> modelSphere;
	std::unique_ptr<ObjModel> modelFighter;

	//出撃シーン用自機
	std::unique_ptr<SortiePlayer> player;
	//天球
	std::unique_ptr<Skydome> skydome;
	//地面
	std::unique_ptr<Ground> ground;
	//背景用(山)
	std::list<std::unique_ptr<Mountain>> mountains;
};