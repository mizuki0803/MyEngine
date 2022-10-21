#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 敵弾
/// </summary>
class EnemyBullet : public ObjObject3d
{
public: // 静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>敵弾</returns>
	static EnemyBullet* Create(ObjModel* model, const Vector3& position, const Vector3& velocity, const float scale = 0.5f);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision();

	//getter
	Vector3 GetWorldPos();
	bool GetIsDead() const { return isDead; }

private: //静的メンバ変数
	//寿命時間
	static const int32_t lifeTime = 180;

private: //メンバ変数
	//速度
	Vector3 velocity;
	//寿命タイマー
	int32_t deathTimer = lifeTime;
	//死亡フラグ
	bool isDead = false;
};