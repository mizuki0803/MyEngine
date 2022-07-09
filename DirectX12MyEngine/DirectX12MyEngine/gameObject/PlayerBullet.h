#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 自機弾
/// </summary>
class PlayerBullet : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>自機弾</returns>
	static PlayerBullet* Create(ObjModel* model, const XMFLOAT3& position, const XMFLOAT3& velocity);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//getter
	bool GetIsDead() const { return isDead; }

private: //静的メンバ変数
	//寿命時間
	static const int32_t lifeTime = 60;

private: //メンバ変数
	//速度
	XMFLOAT3 velocity;
	//寿命タイマー
	int32_t deathTimer = lifeTime;
	//死亡フラグ
	bool isDead = false;
};
