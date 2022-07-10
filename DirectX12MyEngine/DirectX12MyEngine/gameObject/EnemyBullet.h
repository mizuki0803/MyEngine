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
	static EnemyBullet* Create(ObjModel* model, XMFLOAT3& position, XMFLOAT3& velocity);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//getter
	bool GetIsDead() const { return isDead; }

private: //静的メンバ変数
	//寿命時間
	static const int32_t lifeTime = 180;

private: //メンバ変数
	//速度
	XMFLOAT3 velocity;
	//寿命タイマー
	int32_t deathTimer = lifeTime;
	//死亡フラグ
	bool isDead = false;
};