#pragma once
#include "Enemy.h"

/// <summary>
/// 円運動敵
/// </summary>
class CircularEnemy : public Enemy
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>円運動敵</returns>
	static CircularEnemy* Create(ObjModel* model, const Vector3& centerPosition, const float angle, const float length, const float rotSpeed);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 移動
	/// </summary>
	void Move();

private: //静的メンバ変数
	//発射間隔
	static const int fireInterval = 180;

private: //メンバ変数
	//円運動の中心座標
	Vector3 centerPosition;
	//円運動用角度
	float angle;
	//円の半径の長さ
	float length;
	//回転速度
	float rotSpeed;
	//発射タイマー
	int32_t fireTimer = 0;
};