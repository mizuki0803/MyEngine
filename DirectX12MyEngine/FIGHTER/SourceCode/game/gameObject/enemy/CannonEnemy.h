#pragma once
#include "Enemy.h"

/// <summary>
/// 大砲敵
/// </summary>
class CannonEnemy : public Enemy
{
public:
	//行動フェーズ
	enum class Phase {
		Attack,	//攻撃
		Dead,	//死亡
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>大砲敵</returns>
	static CannonEnemy* Create(const Vector3& position);

	//setter
	static void SetModel(ObjModel* model) { CannonEnemy::enemyModel = model; }
	static void SetBreakModel(ObjModel* model) { CannonEnemy::breakModel = model; }

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision() override;

private: //メンバ関数
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 死亡
	/// </summary>
	void Dead();

private: //静的メンバ変数
	//発射間隔
	static const int fireInterval = 300;
	//行動遷移
	static void (CannonEnemy::* actionFuncTable[])();
	//モデル
	static ObjModel* enemyModel;
	//破壊されたモデル
	static ObjModel* breakModel;

private: //メンバ変数
	//行動
	Phase phase = Phase::Attack;
	//発射タイマー
	int32_t fireTimer = 0;
};