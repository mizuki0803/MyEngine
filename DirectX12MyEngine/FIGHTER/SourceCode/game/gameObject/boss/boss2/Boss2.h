#pragma once
#include "Boss2Body.h"
#include "BossNameUI.h"
#include "BossHPUI.h"
#include "Boss2BehaviorTree.h"

//自機クラスを前方宣言
class BasePlayer;

/// <summary>
/// ボス ステージ2ボス
/// </summary>
class Boss2
{
public:
	//行動フェーズ
	enum class Phase {
		Appear,	//登場
		Wait,	//待機
		Dead,	//死亡
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>02</returns>
	static Boss2* Create(const Vector3& position);

	//setter
	static void SetPlayer(BasePlayer* player) { Boss2::player = player; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(const Vector3& position);

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
	/// 衝突時コールバック関数(胴体)
	/// </summary>
	/// <param name="damageNum">ダメージ量</param>
	void OnCollisionBody(const int damageNum, const Vector3& collisionPos, const Vector3& subjectVel);

	/// <summary>
	/// 登場状態
	/// </summary>
	bool AppearModeCount();

	/// <summary>
	/// 登場降下
	/// </summary>
	bool AppearFall();

	/// <summary>
	/// 待機状態
	/// </summary>
	bool WaitMode();

	/// <summary>
	/// 死亡爆発
	/// </summary>
	bool DeadExplosion();

	//getter
	const Phase& GetPhase() { return this->phase; }
	Boss2Body* GetBody() { return body.get(); }
	const bool GetIsDead() { return isDead; }
	const bool GetIsDelete() { return isDelete; }

private: //メンバ関数
	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="damageNum">ダメージ量</param>
	void Damage(const int damageNum);

private: //静的メンバ変数
	//プレイヤー自機
	static BasePlayer* player;
	//登場状態時間
	static const float appearModeTime;
	//待機状態時間
	static const float waitModeTime;

private: //メンバ変数
	//胴体
	std::unique_ptr<Boss2Body> body;
	//ステージ2ボスの行動遷移ビヘイビアツリー
	std::unique_ptr<Boss2BehaviorTree> behaviorTree;
	//行動
	Phase phase = Phase::Appear;
	//登場状態時間タイマー
	int32_t appearModeTimer = 0;
	//待機状態時間タイマー
	int32_t waitModeTimer = 0;
	//死亡状態時間タイマー
	int32_t deadModeTimer = 0;
	//ボス名表示UI
	std::unique_ptr<BossNameUI> bossNameUI;
	//HPUI
	std::unique_ptr<BossHPUI> hpUI;
	//体力
	int HP = 0;
	//死亡フラグ
	bool isDead = false;
	//削除フラグ
	bool isDelete = false;
};
