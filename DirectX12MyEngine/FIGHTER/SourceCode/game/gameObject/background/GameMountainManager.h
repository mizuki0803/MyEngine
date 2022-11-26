#pragma once
#include "Mountain.h"

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
	static GameMountainManager* Create(ObjModel* model, float distance, int startNum);

public: //メンバ変数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize(ObjModel* model, float distance, int startNum);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Vector3& targetPos);

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

private: //メンバ変数
	//山オブジェクト
	std::list<std::unique_ptr<Mountain>> mountains;
	//山モデル
	ObjModel* model = nullptr;
	//中心からの距離
	float distance = 0;
	//基準位置からの山の番号
	int mountainNum = 0;
	//新たな山を設置可能か
	bool isCanCreate = true;
};