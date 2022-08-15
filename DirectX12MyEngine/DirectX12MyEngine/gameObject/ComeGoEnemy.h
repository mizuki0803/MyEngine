#pragma once
#include "Enemy.h"

/// <summary>
/// 到着出発敵
/// </summary>
class ComeGoEnemy : public Enemy
{
public:
	//行動フェーズ
	enum class Phase {
		Come,	//到着
		Attack,	//攻撃
		Go,		//出発
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="startPos">生成座標</param>
	/// <param name="comePos">到着座標</param>
	/// <param name="goPos">出発目標座標</param>
	/// <returns>到着出発敵</returns>
	static ComeGoEnemy* Create(ObjModel* model, const Vector3& startPos, const Vector3& comePos, const Vector3& goTargetPos, const int attackTime);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 到着
	/// </summary>
	void Come();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 出発
	/// </summary>
	void Go();

private: //静的メンバ変数
	//発射間隔
	static const int fireInterval = 120;
	//行動遷移
	static void (ComeGoEnemy::* actionFuncTable[])();

private: //メンバ変数
	//行動
	Phase phase = Phase::Come;
	//生成座標
	Vector3 startPos;
	//到着座標
	Vector3 comePos;
	//到着時間タイマー
	int32_t comeTimer = 0;
	//出発開始座標
	Vector3 goStartPos;
	//出発目標座標
	Vector3 goTargetPos;
	//出発時間タイマー
	int32_t goTimer = 0;
	//攻撃時間タイマー
	int32_t attackTimer = 0;
	//攻撃時間
	int attackTime = 0;
	//発射タイマー
	int32_t fireTimer = 0;
};