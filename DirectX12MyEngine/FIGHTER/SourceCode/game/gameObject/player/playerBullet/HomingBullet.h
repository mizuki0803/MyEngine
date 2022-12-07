#pragma once
#include "PlayerBullet.h"

/// <summary>
/// 自機弾(ホーミング弾)
/// </summary>
class HomingBullet : public PlayerBullet
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ホーミング弾</returns>
	static HomingBullet* Create(const Vector3& position, const Vector3& velocity, const float size, Enemy* enemy);

	//getter
	static const float GetBlastSize() { return HomingBullet::blastSize; }

	//setter
	static void SetBulletModel(ObjModel* model) { HomingBullet::bulletModel = model; }

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision(const Vector3& subjectPos, float subjectSize, bool isToDamage) override;

private: //メンバ関数
	/// <summary>
	/// 地面に衝突したとき
	/// </summary>
	void CollisionGround() override;

private: //静的メンバ変数
	//寿命時間
	static const int32_t lifeTime = 240;
	//死亡時の判定の大きさ
	static const float blastSize;
	//影描画用仮モデル
	static ObjModel* bulletModel;

private: //メンバ変数
	//寿命タイマー
	int32_t deathTimer = lifeTime;
	//ホーミング用タイマー
	float homingTimer = 0;
};
