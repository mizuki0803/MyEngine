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
	/// <param name="model">モデル</param>
	/// <returns>ホーミング弾</returns>
	static HomingBullet* Create(ObjModel* model, const Vector3& position, const Vector3& velocity, Enemy* enemy);

	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ変数
	//ホーミング用タイマー
	float homingTimer = 0;
};
