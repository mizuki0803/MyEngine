#pragma once
#include "BossBody.h"
#include "BossHead.h"
#include "BossHPBar.h"
#include "BossHPFrame.h"
#include "BossBehaviorTree.h"


/// <summary>
/// ボス
/// </summary>
class Boss
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
	/// <returns>ボス</returns>
	static Boss* Create(ObjModel* bodyModel, ObjModel* headModel, const Vector3& position);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(ObjModel* bodyModel, ObjModel* headModel, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// お試し用
	/// </summary>
	/// <returns></returns>
	static bool Otamesi();

	/// <summary>
	/// お試し用
	/// </summary>
	/// <returns></returns>
	static bool Otamesi2();

private: //メンバ関数
	/// <summary>
	/// 行動
	/// </summary>
	void Action();

private: //静的メンバ変数
	//最大体力
	static const int maxHP = 100;

protected: //メンバ変数
	//ボスの行動遷移ビヘイビアツリー
	std::unique_ptr<BossBehaviorTree> behaviorTree;
	//体
	std::unique_ptr<BossBody> bossBody;
	//頭
	std::unique_ptr<BossHead> bossHead;
	//行動
	Phase phase = Phase::Fall;
	//初期座標
	Vector3 startPos;
	//降下する時間タイマー
	int32_t fallTimer = 0;
	//体力
	int HP = maxHP;
	//HPバー
	std::unique_ptr<BossHPBar> hpBar;
	//HPバーフレーム
	std::unique_ptr<BossHPFrame> hpFrame;
	//死亡フラグ
	bool isDead = false;
};
