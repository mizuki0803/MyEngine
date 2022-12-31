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
	/// <param name="isBlackSmoke">黒煙を出すか</param>
	/// <returns>ボス死亡後のエフェクト</returns>
	static BossDeadEffect* Create(const Vector3& deadPos, const bool isBlackSmoke);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//getter
	const bool GetIsExplosionEnd() { return isExplosionEnd; }


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
	//爆発を終了したか
	bool isExplosionEnd = false;
	//黒煙を出すか
	bool isBlackSmoke = false;
};