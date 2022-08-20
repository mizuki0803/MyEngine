#pragma once
#include "BossMainBody.h"
#include "BossAvatar.h"
#include "BossHPBar.h"
#include "BossHPFrame.h"
#include "BossBehaviorTree.h"

//自機クラスを前方宣言
class Player;

/// <summary>
/// ボス
/// </summary>
class Boss
{
public:
	//行動フェーズ
	enum class Phase {
		Fall,	//降下
		Attack,	//攻撃
		Wait,	//待機
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス</returns>
	static Boss* Create(ObjModel* bodyModel, ObjModel* headModel, const Vector3& position);

	//setter
	static void SetPlayer(Player* player) { Boss::player = player; }

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
	/// 攻撃状態
	/// </summary>
	bool AttackMode();

	/// <summary>
	/// 攻撃状態用の角度に変更
	/// </summary>
	bool ChangeRotAttackMode();

	/// <summary>
	/// 待機状態
	/// </summary>
	bool WaitMode();

	/// <summary>
	/// 待機状態用の角度に変更
	/// </summary>
	bool ChangeRotWaitMode();

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

protected: //静的メンバ変数
	//プレイヤー自機
	static Player* player;
	//攻撃状態時間
	static const float attackModeTime;
	//待機状態時間
	static const float waitModeTime;

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
	//攻撃状態時間タイマー
	int32_t attackModeTimer = 0;
	//待機状態時間タイマー
	int32_t waitModeTimer = 0;
	//体力
	int HP = 0;
	//HPバー
	std::unique_ptr<BossHPBar> hpBar;
	//HPバーフレーム
	std::unique_ptr<BossHPFrame> hpFrame;
	//死亡フラグ
	bool isDead = false;
};
