#pragma once
#include "Enemy.h"
#include <array>

/// <summary>
/// 円運動敵
/// </summary>
class CircularEnemy : public Enemy
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>円運動敵</returns>
	static CircularEnemy* Create(const Vector3& centerPosition, const float angle, const float length, const float rotSpeed);

	//setter
	static void SetModel(ObjModel* model) { CircularEnemy::enemyModel = model; }
	static void SetBreakModel(int modelNum, ObjModel* model);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision(const int damageNum) override;

private: //メンバ関数
	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 破壊
	/// </summary>
	void Break() override;

private: //静的メンバ変数
	//最大HP
	static const int maxHP = 1;
	//発射間隔
	static const int fireInterval = 180;
	//モデル
	static ObjModel* enemyModel;
	//破壊時に出すモデル
	static std::array<ObjModel*, 5> breakModels;

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