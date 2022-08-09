#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ボス
/// </summary>
class Boss : public ObjObject3d
{
public:
	//行動フェーズ
	enum class Phase {
		Fall,	//降下
		Stay,	//停止
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>降下敵</returns>
	static Boss* Create(ObjModel* model, const Vector3& position);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 行動
	/// </summary>
	void Action();

protected: //メンバ変数
	//行動
	Phase phase = Phase::Fall;
	//初期座標
	Vector3 startPos;
	//降下する時間タイマー
	int32_t fallTimer = 0;
	//体力
	int HP = 1;
	//死亡フラグ
	bool isDead = false;
};
