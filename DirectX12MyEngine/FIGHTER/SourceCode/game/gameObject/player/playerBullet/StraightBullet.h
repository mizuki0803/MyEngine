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
	/// <param name="position">座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="size">大きさ</param>
	/// <returns>直進弾</returns>
	static StraightBullet* Create(ObjModel* model, const Vector3& position, const Vector3& velocity, const float size = 1.0f);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="subjectPos">衝突した相手の座標</param>
	/// <param name="subjectSize">衝突した相手の大きさ</param>
	/// <param name="isToDamage">ダメージを喰らわせたか</param>
	void OnCollision(const Vector3& subjectPos, float subjectSize, bool isToDamage) override;

private: //メンバ関数
	/// <summary>
	/// 地面に衝突したとき
	/// </summary>
	void CollisionGround() override;

private: //静的メンバ変数
	//ダメージ量
	static const int bulletDamageNum = 1;
	//寿命時間
	static const int32_t lifeTime = 300;

private: //メンバ変数
	//寿命タイマー
	int32_t deathTimer = lifeTime;
};
