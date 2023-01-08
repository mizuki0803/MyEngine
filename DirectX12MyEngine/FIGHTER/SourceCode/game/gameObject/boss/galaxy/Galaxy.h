#pragma once
#include "GalaxyBody.h"
#include "GalaxyBow.h"
#include "GalaxyCannon.h"
#include "BossNameUI.h"
#include "BossHPUI.h"
#include "GalaxyBehaviorTree.h"

//自機クラスを前方宣言
class BasePlayer;

/// <summary>
/// ボス ステージ2ボス
/// </summary>
class Galaxy
{
public:
	//行動フェーズ
	enum class Phase {
		Appear,	//登場
		Attack,	//攻撃
		Wait,	//待機
		AttackPartChange, //攻撃するパーツを変更
		Dead,	//死亡
	};

	//攻撃するパーツのフェーズ
	enum class AttackPartPhase {
		None,	//未設定
		Cannon,	//大砲
		Front,	//船首
	};

	//攻撃種類
	enum class AttackType {
		None,				//未設定
		RapidFireCannon,	//速射(大砲)
		a,					//あ
		FlamethrowerBow,	//火炎放射(船首)
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="bornPos">生成座標</param>
	/// <param name="basePos">基準座標</param>
	/// <returns>ギャラクシー</returns>
	static Galaxy* Create(const Vector3& bornPos, const Vector3& basePos);

	//setter
	static void SetPlayer(BasePlayer* player) { Galaxy::player = player; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="bornPos">生成座標</param>
	/// <param name="basePos">基準座標</param>
	/// <returns>成否</returns>
	bool Initialize(const Vector3& bornPos, const Vector3& basePos);

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
	/// 衝突時コールバック関数(船首)
	/// </summary>
	/// <param name="damageNum">ダメージ量</param>
	/// <param name="collisionPos">衝突座標</param>
	void OnCollisionBow(const int damageNum, const Vector3& collisionPos);

	/// <summary>
	/// 衝突時コールバック関数(大砲)
	/// </summary>
	/// <param name="cannon">どの大砲か</param>
	/// <param name="damageNum">ダメージ量</param>
	/// <param name="collisionPos">衝突座標</param>
	void OnCollisionCannon(GalaxyCannon* cannon, const int damageNum, const Vector3& collisionPos);

	/// <summary>
	/// 登場状態
	/// </summary>
	bool AppearMode();

	/// <summary>
	/// 登場状態のUI生成
	/// </summary>
	bool AppearUICreate();

	/// <summary>
	/// 攻撃状態
	/// </summary>
	bool AttackMode();

	/// <summary>
	/// 弾発射座標を更新
	/// </summary>
	bool UpdateBulletShotPos();

	/// <summary>
	/// 攻撃内容設定を開始するか
	/// </summary>
	bool AttackTypeSelectStart();

	/// <summary>
	/// 攻撃内容:速射(大砲)を設定するか
	/// </summary>
	bool AttackTypeRapidFireCannonSelect();

	/// <summary>
	/// 攻撃内容:「あ」を設定するか
	/// </summary>
	bool AttackTypeASelect();

	/// <summary>
	/// 攻撃内容:火炎放射(船首)を設定するか
	/// </summary>
	bool AttackTypeFlamethrowerBowSelect();

	/// <summary>
	/// 攻撃内容;速射(大砲)
	/// </summary>
	bool AttackTypeRapidFireCannon();

	/// <summary>
	/// 攻撃内容;「あ」
	/// </summary>
	bool AttackTypeA();

	/// <summary>
	/// 攻撃内容:火炎放射(船首)
	/// </summary>
	bool AttackTypeFlamethrowerBow();

	/// <summary>
	/// 待機状態
	/// </summary>
	bool WaitMode();

	/// <summary>
	/// 攻撃するパーツ変更状態
	/// </summary>
	bool AttackPartChangeMode();

	/// <summary>
	/// 死亡爆発
	/// </summary>
	bool DeadExplosion();

	//getter
	const Phase& GetPhase() { return this->phase; }
	GalaxyBody* GetBody() { return body.get(); }
	GalaxyBow* GetBow() { return bow.get(); }
	const std::list<std::unique_ptr<GalaxyCannon>>& GetCannons() { return cannons; };
	const bool GetIsDead() { return isDead; }
	const bool GetIsDelete() { return isDelete; }

private: //メンバ関数
	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="damageNum">ダメージ量</param>
	void Damage(const int damageNum);

	/// <summary>
	/// 攻撃状態開始処理
	/// </summary>
	void AttackModeStart();

	/// <summary>
	/// 大砲が全滅したかチェックする
	/// </summary>
	void CheckAllCannonDead();

private: //静的メンバ変数
	//プレイヤー自機
	static BasePlayer* player;
	//待機状態時間
	static const float waitModeTime;

private: //メンバ変数
	//胴体
	std::unique_ptr<GalaxyBody> body;
	//船首
	std::unique_ptr<GalaxyBow> bow;
	//大砲
	std::list<std::unique_ptr<GalaxyCannon>> cannons;
	//ステージ2ボスの行動遷移ビヘイビアツリー
	std::unique_ptr<GalaxyBehaviorTree> behaviorTree;
	//行動
	Phase phase = Phase::Appear;
	//生成座標
	Vector3 bornPos = {};
	//基準座標
	Vector3 basePos = {};
	//攻撃するパーツフェーズ
	AttackPartPhase attackPartPhase = AttackPartPhase::None;
	//攻撃内容
	AttackType attackType = AttackType::None;
	//1つ前に発動した攻撃内容
	AttackType preAttackType = AttackType::None;
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
