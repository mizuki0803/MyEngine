#pragma once
#include "ObjObject3d.h"

class Enemy;

/// <summary>
/// 自機弾
/// </summary>
class PlayerBullet : public ObjObject3d
{
public: //enum
	enum class BulletType {
		None,		//未設定
		Straight,	//直進弾
		Homing,		//ホーミング弾
	};

public: //静的メンバ関数
	//setter
	static void SetIsGroundMode(bool isGroundMode) { PlayerBullet::isGroundMode = isGroundMode; }

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	virtual void OnCollision(const Vector3& subjectPos, float subjectSize, bool isToDamage = true) = 0;

	//getter
	Vector3 GetWorldPos();
	BulletType GetBulletType() { return bulletType; }
	const Vector3& GetVelocity() { return velocity; }
	bool GetIsDead() const { return isDead; }
	Enemy* GetEnemy() { return enemy; }

	void SetEnemy(Enemy* enemy) { this->enemy = enemy; }

protected: //メンバ関数
	/// <summary>
	/// 地面に衝突したとき
	/// </summary>
	virtual void CollisionGround() = 0;

protected: //静的メンバ変数
	//地面ありの行動を行うか
	static bool isGroundMode;

protected: //メンバ変数
	//弾の種類
	BulletType bulletType = BulletType::None;
	//速度
	Vector3 velocity;
	//死亡フラグ
	bool isDead = false;
	//ホーミング用敵
	Enemy* enemy = nullptr;
};
