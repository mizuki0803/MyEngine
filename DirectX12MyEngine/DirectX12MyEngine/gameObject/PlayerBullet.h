#pragma once
#include "ObjObject3d.h"

class Enemy;

/// <summary>
/// 自機弾
/// </summary>
class PlayerBullet : public ObjObject3d
{
public: //enum
	enum BulletType {
		None,		//未設定
		Straight,	//直進弾
		Homing,		//ホーミング弾
	};

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
	BulletType GetBulletType() { return bulletType; }
	bool GetIsDead() const { return isDead; }
	Enemy* GetEnemy() { return enemy; }

	void SetEnemy(Enemy* enemy) { this->enemy = enemy; }

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
