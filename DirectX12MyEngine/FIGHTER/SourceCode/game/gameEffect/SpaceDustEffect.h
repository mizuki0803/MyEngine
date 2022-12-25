#pragma once
#include "ObjObject3d.h"
#include "Player.h"
#include "GameCamera.h"

/// <summary>
/// 宇宙塵エフェクト
/// </summary>
class SpaceDustEffect : public ObjObject3d
{
public: // 静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>宇宙塵エフェクト</returns>
	static SpaceDustEffect* Create(const Vector3& position, const Vector3& rotation);

	//setter
	static void SetSpaceDustModel(ObjModel* model) { SpaceDustEffect::spaceDustModel = model; };
	static void SetPlayer(Player* player) { SpaceDustEffect::player = player; }
	static void SetGameCamera(GameCamera* gameCamera) { SpaceDustEffect::gameCamera = gameCamera; }
	static void SetIsScrollMode(const bool isScrollMode) { SpaceDustEffect::isScrollMode = isScrollMode; }

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//getter
	bool GetIsDelete() const { return isDelete; }

private: //メンバ関数
	/// <summary>
	/// 画面手前まで行ったら削除する処理
	/// </summary>
	void FrontOfScreenDelete();

protected: //静的メンバ変数
	//塵モデル
	static ObjModel* spaceDustModel;
	//自機
	static Player* player;
	//ゲームカメラ
	static GameCamera* gameCamera;
	//1粒の大きさ
	static const float spaceDustSize;
	//宇宙塵エフェクトがスクロール状態か
	static bool isScrollMode;

protected: //メンバ変数
	//削除フラグ
	bool isDelete = false;
};