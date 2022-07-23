#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 自機弾
/// </summary>
class PlayerBullet : public ObjObject3d
{
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision();

	//getter
	Vector3 GetWorldPos();
	bool GetIsDead() const { return isDead; }

protected: //メンバ変数
	//速度
	Vector3 velocity;
	//死亡フラグ
	bool isDead = false;
};
