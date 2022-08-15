#pragma once
#include "Enemy.h"

/// <summary>
/// 確認用敵
/// </summary>
class DemoEnemy : public Enemy
{
public:
	//行動フェーズ
	enum class Phase {
		Previous,	//前進
		Back,		//後退
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>確認用敵</returns>
	static DemoEnemy* Create(ObjModel* model, const Vector3& position, const Vector3& velocity);

public: //メンバ関数
/// <summary>
/// 初期化
/// </summary>
	bool Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;


private: //メンバ関数
	/// <summary>
	/// 前進
	/// </summary>
	void Previous();

	/// <summary>
	/// 後退
	/// </summary>
	void Back();

private: //静的メンバ変数
	//発射間隔
	static const int fireInterval = 60;
	//行動遷移
	static void (DemoEnemy::* actionFuncTable[])();

private: //メンバ変数
	//速度
	Vector3 velocity;
	//行動フェーズ
	Phase phase = Phase::Previous;
	//発射タイマー
	int32_t fireTimer = 0;
};
