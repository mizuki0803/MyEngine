#pragma once
#include "Building.h"
#include <array>

//自機
class BasePlayer;
//ゲームカメラ
class BaseGameCamera;

/// <summary>
/// ゲームで使うビル管理
/// </summary>
class GameBuildingManager
{
public:  //静的メンバ変数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="centerDistance">中心からの距離</param>
	/// <param name="objectDistance">オブジェクト同士の距離</param>
	/// <param name="startNum">開始時にセットする数</param>
	/// <param name="basePos">基準座標</param>
	/// <returns>ゲームで使うビル管理</returns>
	static GameBuildingManager* Create(float centerDistance, float objectDistance, int startNum, const Vector3& basePos);

	//setter
	static void SetBuidingModel(int modelNum, ObjModel* model);
	static void SetPlayer(BasePlayer* player) { GameBuildingManager::player = player; }
	static void SetGameCamera(BaseGameCamera* gameCamera) { GameBuildingManager::gameCamera = gameCamera; }

public: //メンバ変数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="centerDistance">中心からの距離</param>
	/// <param name="objectDistance">オブジェクト同士の距離</param>
	/// <param name="startNum">開始時にセットする数</param>
	/// <param name="basePos">基準座標</param>
	/// <returns>成否</returns>
	bool Initialize(float centerDistance, float objectDistance, int startNum, const Vector3& basePos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 影用光源ライトから見た視点での描画
	/// </summary>
	void DrawLightCameraView();

	/// <summary>
	/// ゲームクリア後に後ろを向くのでオブジェクトを大量に追加
	/// </summary>
	void CreateBehindObjects();

	//setter
	void SetIsScroll(bool isScroll) { this->isScrollMode = isScroll; }

private: //メンバ関数
	/// <summary>
	/// 新たなビル生成
	/// </summary>
	void CreateNewBuilding();

	/// <summary>
	/// 次にセットするビルのモデル番号を取得
	/// </summary>
	/// <param name="lastSetModelNum">1つ前にセットしたモデル番号</param>
	/// <returns>次にセットするビルのモデル番号</returns>
	int NextCreateModelNum(int& lastSetModelNum);

	/// <summary>
	/// スクロール状態
	/// </summary>
	void ScrollMode();

private: //静的メンバ変数
	//ビルモデル
	static std::array<ObjModel*, 2> buildingModels;
	//自機
	static BasePlayer* player;
	//ゲームカメラ
	static BaseGameCamera* gameCamera;

private: //メンバ変数
	//ビルオブジェクト
	std::list<std::unique_ptr<Building>> buildings;
	//基準の座標
	Vector3 basePos;
	//左側に最後に設置したビルモデル番号
	int lastLeftSetModelNum = 0;
	//右側に最後に設置したビルモデル番号
	int lastRightSetModelNum = 1;
	//中心からの距離
	float centerDistance = 0;
	//オブジェクト同士の距離
	float objectDistance = 0;
	//基準位置からのビルの番号
	int buildingNum = 0;
	//新たなビルを設置可能か
	bool isCanCreate = true;
	//最後に生成したビルの座標
	Vector3 lastCreateBuildingPos;
	//ビルがスクロール状態か
	bool isScrollMode = false;
	//自機の後ろまで行ったら削除するか
	bool isPlayerBehindDelete = true;
};