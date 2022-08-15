#pragma once
#include "Enemy.h"

/// <summary>
/// 降下敵
/// </summary>
class FallEnemy : public Enemy
{
public:
	//行動フェーズ
	enum class Phase {
		Fall,	//降下
		Attack,	//攻撃
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>降下敵</returns>
	static FallEnemy* Create(ObjModel* model, const Vector3& position);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 降下
	/// </summary>
	void Fall();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

private: //静的メンバ変数
	//発射間隔
	static const int fireInterval = 180;
	//行動遷移
	static void (FallEnemy::* actionFuncTable[])();

private: //メンバ変数
	//行動
	Phase phase = Phase::Fall;
	//初期座標
	Vector3 startPos;
	//降下する時間タイマー
	int32_t fallTimer = 0;
	//発射タイマー
	int32_t fireTimer = 0;
};