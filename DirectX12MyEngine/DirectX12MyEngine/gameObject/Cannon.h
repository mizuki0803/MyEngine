#pragma once
#include "Enemy.h"

/// <summary>
/// 敵:大砲
/// </summary>
class Cannon : public Enemy
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>大砲</returns>
	static Cannon* Create(ObjModel* model, const Vector3& position);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 画面手前まで行ったら削除する処理
	/// </summary>
	void FrontOfScreenDelete();

private: //静的メンバ変数
	//発射間隔
	static const int fireInterval = 300;

private: //メンバ変数
	//発射タイマー
	int32_t fireTimer = 0;
};