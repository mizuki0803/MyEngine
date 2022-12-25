#pragma once
#include "Enemy.h"
#include <array>

/// <summary>
/// 隕石敵(破壊可能)
/// </summary>
class MeteoriteEnemy : public Enemy
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
	static MeteoriteEnemy* Create(const Vector3& position, const Vector3& rotation, float size, const Vector3& velocity, const Vector3& rotSpeed, int HP);

	//setter
	static void SetModel(ObjModel* model) { MeteoriteEnemy::meteoriteModel = model; }

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
	/// 死亡
	/// </summary>
	void Dead();

	/// <summary>
	/// 破壊
	/// </summary>
	void Break() override;

private: //静的メンバ変数
	//モデル
	static ObjModel* meteoriteModel;

private: //メンバ変数
	//速度
	Vector3 velocity = {};
	//回転速度
	Vector3 rotSpeed = {};
	//死亡後用タイマー
	int32_t deadTimer = 0;

};