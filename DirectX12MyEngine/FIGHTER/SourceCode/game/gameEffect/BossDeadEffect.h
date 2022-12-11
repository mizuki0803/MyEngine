#pragma once
#include "Vector3.h"

/// <summary>
/// ボス死亡後のエフェクト
/// </summary>
class BossDeadEffect
{
public: // 静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="position">ボス死亡座標</param>
	/// <returns>ボス死亡後のエフェクト</returns>
	static BossDeadEffect* Create(const Vector3& deadPos);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private: //メンバ関数
	/// <summary>
	/// 爆発
	/// </summary>
	void Explosion();

	/// <summary>
	/// 黒煙
	/// </summary>
	void BlackSmoke();

private: //メンバ変数
	//ボス死亡座標
	Vector3 deadPos;
	//経過時間
	int32_t timer = 0;
	//爆発回数
	int explosionCount = 0;
	//削除フラグ
	bool isDelete = false;
};