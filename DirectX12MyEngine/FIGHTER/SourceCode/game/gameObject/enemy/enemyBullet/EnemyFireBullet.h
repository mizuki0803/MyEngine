#pragma once
#include "EnemyBullet.h"

/// <summary>
/// 敵火炎弾
/// </summary>
class EnemyFireBullet : public EnemyBullet
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>	
	/// <param name="position">座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="size">大きさ</param>
	/// <returns>敵火炎弾</returns>
	static EnemyFireBullet* Create(const Vector3& position, const Vector3& velocity, const float size);


public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision() override;

private: //静的メンバ変数
	//寿命時間
	static const int32_t lifeTime = 90;
};
