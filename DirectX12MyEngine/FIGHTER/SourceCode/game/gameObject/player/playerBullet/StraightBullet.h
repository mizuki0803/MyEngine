#pragma once
#include "PlayerBullet.h"

/// <summary>
/// 自機弾(直進弾)
/// </summary>
class StraightBullet : public PlayerBullet
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>直進弾</returns>
	static StraightBullet* Create(ObjModel* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision(const Vector3& subjectsize, float subjectSize) override;

private: //メンバ関数
	/// <summary>
	/// 地面に衝突したとき
	/// </summary>
	void CollisionGround() override;

private: //静的メンバ変数
	//寿命時間
	static const int32_t lifeTime = 300;

private: //メンバ変数
	//寿命タイマー
	int32_t deathTimer = lifeTime;
};
