#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ギャラクシー(プロペラ)
/// </summary>
class GalaxyPropeller : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="parent">親オブジェクト</param> 
	/// <param name="position">座標</param>
	/// <param name="rotation">初期回転角</param>
	/// <param name="rotSpeed">回転速度</param>
	/// <param name="size">大きさ</param>
	/// <returns>ギャラクシー(プロペラ)</returns>
	static GalaxyPropeller* Create(ObjObject3d* parent, const Vector3& position, const Vector3& rotation, const float rotSpeed, const float size);

	//setter
	static void SetPropellerModel(ObjModel* model) { GalaxyPropeller::propellerModel = model; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="parent">親オブジェクト</param> 
	/// <param name="position">座標</param>
	/// <param name="rotation">初期回転角</param>
	/// <param name="rotSpeed">回転速度</param>
	/// <param name="size">大きさ</param>
	/// <returns>成否</returns>
	bool Initialize(ObjObject3d* parent, const Vector3& position, const Vector3& rotation, const float rotSpeed, const float size);

	/// <summary>
	/// 初期化
	/// </summary>
	void Update() override;

	//setter
	void SetIsRotation(const float isRotation) { this->isRotation = isRotation; }

protected: //メンバ関数
	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();

protected: //静的メンバ変数
	//プロペラのモデル
	static ObjModel* propellerModel;
	//ダメージ状態の色
	static const XMFLOAT4 damageColor;

protected: //メンバ変数
	//回転するか
	bool isRotation = true;
	//回転速度
	float rotSpeed = 0;
	//ダメージフラグ
	bool isDamage = false;
	//ダメージ状態タイマー
	int32_t damageTimer = 0;
	//ダメージ色か
	bool isDamageColor = false;
};