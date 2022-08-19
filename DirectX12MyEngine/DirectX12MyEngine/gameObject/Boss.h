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
	/// 衝突時コールバック関数(本体)
	/// </summary>
	/// <param name="bossAvatars">衝突した分身</param>
	/// <param name="damageNum">ダメージ量</param>
	void OnCollisionMainBody(const int damageNum);

	/// <summary>
	/// 衝突時コールバック関数(分身)
	/// </summary>
	/// <param name="avatars">衝突した分身</param>
	/// <param name="damageNum">ダメージ量</param>
	void OnCollisionAvator(BossAvatar* avatar, const int damageNum);

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
	BossMainBody* GetMainBody() { return mainBody.get(); }
	const std::list<std::unique_ptr<BossAvatar>>& GetAvators() { return avatars; };

private: //メンバ関数
	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="damageNum">ダメージ量</param>
	void Damage(const int damageNum);

private: //メンバ変数
	//ボスの行動遷移ビヘイビアツリー
	std::unique_ptr<BossBehaviorTree> behaviorTree;
	//本体
	std::unique_ptr<BossMainBody> mainBody;
	//分身
	std::list<std::unique_ptr<BossAvatar>> avatars;
	//行動
	Phase phase = Phase::Fall;
	//降下する時間タイマー
	int32_t fallTimer = 0;
	//体力
	int HP = 0;
	//HPバー
	std::unique_ptr<BossHPBar> hpBar;
	//HPバーフレーム
	std::unique_ptr<BossHPFrame> hpFrame;
	//死亡フラグ
	bool isDead = false;
};
