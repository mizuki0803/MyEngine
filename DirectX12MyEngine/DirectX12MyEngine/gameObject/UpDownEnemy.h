#pragma once
#include "Enemy.h"

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
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>上下移動敵</returns>
	static UpDownEnemy* Create(ObjModel* model, const Vector3& position);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 上昇にブレーキをかける
	/// </summary>
	void UpBrake();

	/// <summary>
	/// 下降にブレーキをかける
	/// </summary>
	void DownBrake();

private: //静的メンバ変数
	//発射間隔
	static const int fireInterval = 180;
	//行動遷移
	static void (UpDownEnemy::* actionFuncTable[])();

private: //メンバ変数
	//行動
	Phase phase = Phase::UpBrake;
	//速度
	Vector3 velocity = { 0, 1.0f, -0.02f };
	//発射タイマー
	int32_t fireTimer = 0;
};