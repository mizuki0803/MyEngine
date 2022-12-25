#pragma once
#include "Enemy.h"
#include <array>

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
		Dead,	//死亡
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
	static ComeGoEnemy* Create(const Vector3& startPos, const Vector3& comePos, const Vector3& goTargetPos, const int attackTime);

	//setter
	static void SetAttackMoveSpeed(float moveSpeed) { ComeGoEnemy::attackMoveSpeed = moveSpeed; }
	static void SetModel(ObjModel* model) { ComeGoEnemy::enemyModel = model; }
	static void SetBreakModel(int modelNum, ObjModel* model);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision(const int damageNum) override;

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
	/// Z軸ゆらゆら回転
	/// </summary>
	void SwayZ();

	/// <summary>
	/// 出発
	/// </summary>
	void Go();

	/// <summary>
	/// 死亡
	/// </summary>
	void Dead();

	/// <summary>
	/// 破壊
	/// </summary>
	void Break() override;

private: //静的メンバ変数
	//最大HP
	static const int maxHP = 1;
	//発射間隔
	static const int fireInterval = 120;
	//行動遷移
	static void (ComeGoEnemy::* actionFuncTable[])();
	//攻撃時に移動する速さ
	static float attackMoveSpeed;
	//モデル
	static ObjModel* enemyModel;
	//破壊時に出すモデル
	static std::array<ObjModel*, 5> breakModels;

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
	//z軸ゆらゆら回転が右回転か
	bool isRotZRight = true;
	//死亡時墜落速度
	Vector3 crashVel = { 0, 0.05f, 0.05f };
	//死亡時墜落回転速度
	Vector3 crashRotVel;
};