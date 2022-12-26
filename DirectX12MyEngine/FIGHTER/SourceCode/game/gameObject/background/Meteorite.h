#pragma once
#include "ObjObject3d.h"
#include "BaseGameCamera.h"

/// <summary>
/// 隕石(破壊不可背景用)
/// </summary>
class Meteorite : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="rotation">回転角</param>
	/// <param name="size">サイズ</param>
	/// <param name="velocity">移動速度</param>
	/// <param name="rotSpeed">回転速度</param>
	/// <returns>隕石(破壊不可背景用)</returns>
	static Meteorite* Create(const Vector3& position, const Vector3& rotation, float size, const Vector3& velocity, const Vector3& rotSpeed);

	//setter
	static void SetMeteoriteModel(ObjModel* model) { Meteorite::meteoriteModel = model; };
	static void SetGameCamera(BaseGameCamera* gameCamera) { Meteorite::gameCamera = gameCamera; }

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//getter
	bool GetIsDelete() { return isDelete; }

private: //メンバ関数
	/// <summary>
	/// 画面手前まで行ったら削除する処理
	/// </summary>
	void FrontOfScreenDelete();

private: //静的メンバ変数
	//隕石(破壊不可背景用)モデル
	static ObjModel* meteoriteModel;
	//ゲームカメラ
	static BaseGameCamera* gameCamera;

private: //メンバ変数
	//移動速度
	Vector3 velocity = {};
	//回転速度
	Vector3 rotSpeed = {};
	//削除フラグ
	bool isDelete = false;
};
