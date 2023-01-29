#pragma once
#include "Enemy.h"
#include <array>

/// <summary>
/// 上下移動敵
/// </summary>
class UpDownEnemy : public Enemy
{
public:
	//行動フェーズ
	enum class Phase {
		UpBrake,	//上昇にブレーキをかける
		DownBrake,	//下降にブレーキをかける
		Dead,		//死亡
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="position">座標</param>
	/// <returns>上下移動敵</returns>
	static UpDownEnemy* Create(const Vector3& position);

	//setter
	static void SetModel(ObjModel* model) { UpDownEnemy::enemyModel = model; }
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
	/// 上昇にブレーキをかける
	/// </summary>
	void UpBrake();

	/// <summary>
	/// 下降にブレーキをかける
	/// </summary>
	void DownBrake();

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
	static const int fireInterval = 100;
	//行動遷移
	static void (UpDownEnemy::* actionFuncTable[])();
	//モデル
	static ObjModel* enemyModel;
	//破壊時に出すモデル
	static std::array<ObjModel*, 5> breakModels;

private: //メンバ変数
	//行動
	Phase phase = Phase::UpBrake;
	//速度
	Vector3 velocity = { 0, 1.0f, -0.02f };
	//発射タイマー
	int32_t fireTimer = 0;
	//死亡時墜落速度
	Vector3 crashVel = { 0, 0.5f, 0.5f };
	//死亡時墜落回転速度
	Vector3 crashRotVel;
};