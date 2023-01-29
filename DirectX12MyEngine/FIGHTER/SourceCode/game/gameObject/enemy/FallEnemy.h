#pragma once
#include "Enemy.h"
#include <array>

/// <summary>
/// 降下敵
/// </summary>
class FallEnemy : public Enemy
{
public:
	//行動フェーズ
	enum class Phase {
		Fall,	//降下
		Rotate,	//回転
		Dead,	//死亡
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="stayPos">停止座標</param>
	/// <param name="fallNum">降下量</param>
	/// <returns>降下敵</returns>
	static FallEnemy* Create(const Vector3& stayPos, const float fallNum);

	//setter
	static void SetModel(ObjModel* model) { FallEnemy::enemyModel = model; }
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
	/// 降下
	/// </summary>
	void Fall();

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

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
	//行動遷移
	static void (FallEnemy::* actionFuncTable[])();
	//モデル
	static ObjModel* enemyModel;
	//破壊時に出すモデル
	static std::array<ObjModel*, 5> breakModels;

private: //メンバ変数
	//行動
	Phase phase = Phase::Fall;
	//停止座標
	Vector3 stayPos;
	//降下する値
	float fallNum = 0;
	//降下する時間タイマー
	int32_t fallTimer = 0;
	//死亡時墜落速度
	Vector3 crashVel = { 0, 0.05f, 0 };
	//死亡時墜落回転速度
	Vector3 crashRotVel;
};