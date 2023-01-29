#pragma once
#include "Enemy.h"
#include <array>

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
	/// <param name="position">座標</param>
	/// <returns>大砲敵</returns>
	static CannonEnemy* Create(const Vector3& position);

	//setter
	static void SetModel(ObjModel* model) { CannonEnemy::enemyModel = model; }
	static void SetBreakModel(int modelNum, ObjModel* model);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="damageNum">ダメージ量</param>
	void OnCollision(const int damageNum) override;

private: //メンバ関数
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

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
	static const int fireInterval = 300;
	//行動遷移
	static void (CannonEnemy::* actionFuncTable[])();
	//モデル
	static ObjModel* enemyModel;
	//破壊時に出すモデル
	static std::array<ObjModel*, 5> breakModels;

private: //メンバ変数
	//行動
	Phase phase = Phase::Attack;
	//発射タイマー
	int32_t fireTimer = 0;
	//死亡時墜落速度
	Vector3 crashVel = { 0, 0.1f, 0.0025f };
	//死亡時墜落回転速度
	Vector3 crashRotVel;
};