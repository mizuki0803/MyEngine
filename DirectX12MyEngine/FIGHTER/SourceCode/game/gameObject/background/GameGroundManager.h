#pragma once
#include "ground.h"
#include <array>

//自機
class Player;
//ゲームカメラ
class GameCamera;

/// <summary>
/// ゲームで使う地面管理
/// </summary>
class GameGroundManager
{
public:  //静的メンバ変数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ゲームで使う地面管理</returns>
	static GameGroundManager* Create(ObjModel* model);

	//setter
	static void SetPlayer(Player* player) { GameGroundManager::player = player; }
	static void SetGameCamera(GameCamera* gameCamera) { GameGroundManager::gameCamera = gameCamera; }

public: //メンバ変数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize(ObjModel* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//setter
	void SetIsScroll(bool isScroll) { this->isScrollMode = isScroll; }

private: //メンバ関数
	/// <summary>
	/// 後ろに行った地面を前に持ってくる
	/// </summary>
	void SwapGroundPos();

	/// <summary>
	/// スクロール状態
	/// </summary>
	void ScrollMode();

private: //静的メンバ変数
	//自機
	static Player* player;
	//ゲームカメラ
	static GameCamera* gameCamera;

private: //メンバ変数
	//地面オブジェクト
	std::array<std::unique_ptr<Ground>, 2> grounds;
	//地面モデル
	ObjModel* model = nullptr;
	//地面がスクロール状態か
	bool isScrollMode = false;
};