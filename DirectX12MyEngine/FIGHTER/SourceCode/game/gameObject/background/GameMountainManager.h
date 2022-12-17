#pragma once
#include "Mountain.h"

//自機
class Player;
//ゲームカメラ
class GameCamera;

/// <summary>
/// ゲームで使う山管理
/// </summary>
class GameMountainManager
{
public:  //静的メンバ変数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ゲームで使う山管理</returns>
	static GameMountainManager* Create(ObjModel* model, float centerDistance, float objectDistance, int startNum);

	//setter
	static void SetPlayer(Player* player) { GameMountainManager::player = player; }
	static void SetGameCamera(GameCamera* gameCamera) { GameMountainManager::gameCamera = gameCamera; }
	static void SetIsScroll(bool isScroll) { GameMountainManager::isScrollMode = isScroll; }

public: //メンバ変数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize(ObjModel* model, float centerDistance, float objectDistance, int startNum);

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

private: //メンバ関数
	/// <summary>
	/// 新たな山生成
	/// </summary>
	void CreateNewMountain();

	/// <summary>
	/// スクロール状態
	/// </summary>
	void ScrollMode();

private: //静的メンバ変数
	//自機
	static Player* player;
	//ゲームカメラ
	static GameCamera* gameCamera;
	//山がスクロール状態か
	static bool isScrollMode;

private: //メンバ変数
	//山オブジェクト
	std::list<std::unique_ptr<Mountain>> mountains;
	//山モデル
	ObjModel* model = nullptr;
	//中心からの距離
	float centerDistance = 0;
	//オブジェクト同士の距離
	float objectDistance = 0;
	//基準位置からの山の番号
	int mountainNum = 0;
	//新たな山を設置可能か
	bool isCanCreate = true;
	//最後に生成した山の座標
	Vector3 lastCreateMountainPos;
};