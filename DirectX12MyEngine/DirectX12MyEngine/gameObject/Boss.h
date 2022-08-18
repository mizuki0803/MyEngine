#pragma once
#include "BossMainBody.h"
#include "BossAvatar.h"
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
	/// 降下
	/// </summary>
	bool Fall();

	/// <summary>
	/// お試し用
	/// </summary>
	/// <returns></returns>
	bool Otamesi();

	/// <summary>
	/// お試し用
	/// </summary>
	/// <returns></returns>
	bool Otamesi2();

	/// <summary>
	/// お試し用
	/// </summary>
	/// <returns></returns>
	bool Otamesi3();

	/// <summary>
	/// お試し用
	/// </summary>
	/// <returns></returns>
	bool Otamesi4();

	//getter
	const Phase& GetPhase() { return this->phase; }

private: //静的メンバ変数
	//最大体力
	static const int maxHP = 100;

protected: //メンバ変数
	//ボスの行動遷移ビヘイビアツリー
	std::unique_ptr<BossBehaviorTree> behaviorTree;
	//本体
	std::unique_ptr<BossMainBody> bossMainBody;
	//分身
	std::list<std::unique_ptr<BossAvatar>> bossAvatars;
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
