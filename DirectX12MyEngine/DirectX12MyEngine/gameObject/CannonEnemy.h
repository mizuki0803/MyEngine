#pragma once
#include "Enemy.h"

/// <summary>
/// 大砲敵
/// </summary>
class CannonEnemy : public Enemy
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>大砲敵</returns>
	static CannonEnemy* Create(ObjModel* model, const Vector3& position);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //静的メンバ変数
	//発射間隔
	static const int fireInterval = 300;

private: //メンバ変数
	//発射タイマー
	int32_t fireTimer = 0;
};