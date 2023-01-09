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
	/// <param name="explosionSize">爆発の大きさ</param>
	/// <param name="isBlackSmoke">黒煙を出すか</param>
	/// <param name="isGround">地面用の爆発をするか</param>
	/// <returns>ボス死亡後のエフェクト</returns>
	static BossDeadEffect* Create(const Vector3& deadPos, const float explosionSize,  const bool isBlackSmoke, const bool isGround);

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
	//爆発の大きさ
	float explosionSize = 0;
	//爆発回数
	int explosionCount = 0;
	//地面用爆発か
	bool isGround = false;
	//爆発を終了したか
	bool isExplosionEnd = false;
	//黒煙を出すか
	bool isBlackSmoke = false;
};