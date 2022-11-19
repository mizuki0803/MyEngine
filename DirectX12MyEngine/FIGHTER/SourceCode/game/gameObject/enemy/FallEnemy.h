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
		Rotate,	//回転
		Dead,	//死亡
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>降下敵</returns>
	static FallEnemy* Create(ObjModel* model, const Vector3& stayPos, const float fallNum);

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

private: //静的メンバ変数
	//行動遷移
	static void (FallEnemy::* actionFuncTable[])();

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