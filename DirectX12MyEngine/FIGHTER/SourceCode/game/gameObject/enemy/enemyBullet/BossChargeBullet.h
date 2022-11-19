#pragma once
#include "EnemyBullet.h"

/// <summary>
/// ボスチャージ弾
/// </summary>
class BossChargeBullet : public EnemyBullet
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ボスチャージ弾</returns>
	static BossChargeBullet* Create(const Vector3& position, const Vector3& velocity, const float size);


public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision() override;
};
