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
		Dead,	//死亡
	};

	//行動フェーズ
	enum class AttackType {
		None,				//未設定
		Tracking,			//追従
		SuperGiantBullet,	//超巨大弾
		Rotate,				//回転からの発射
		AvatarBodyBlow,		//分身体当たり
		AvatarGatling,		//分身ガトリング砲
		AvatarGiantBullet,	//分身巨大弾
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ボス</returns>
	static Boss* Create(const Vector3& position);

	//setter
	static void SetPlayer(Player* player) { Boss::player = player; }

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
	void OnCollisionAvatar(BossAvatar* avatar, const int damageNum);

	/// <summary>
	/// 降下状態
	/// </summary>
	bool FallMode();

	/// <summary>
	/// 攻撃内容設定を開始するか
	/// </summary>
	/// <returns></returns>
	bool AttackTypeSelectStart();

	/// <summary>
	/// 攻撃内容:追従を設定するか
	/// </summary>
	/// <returns></returns>
	bool AttackTypeTrackingSelect();

	/// <summary>
	/// 攻撃内容:超巨大弾を設定するか
	/// </summary>
	/// <returns></returns>
	bool AttackTypeSuperGiantBulletSelect();

	/// <summary>
	/// 攻撃内容:回転を設定するか
	/// </summary>
	/// <returns></returns>
	bool AttackTypeRotateSelect();

	/// <summary>
	/// 攻撃内容:分身体当たりを設定するか
	/// </summary>
	/// <returns></returns>
	bool AttackTypeAvatarBodyBlowSelect();

	/// <summary>
	/// 攻撃内容:分身ガトリング砲を設定するか
	/// </summary>
	/// <returns></returns>
	bool AttackTypeAvatarGatlingSelect();

	/// <summary>
	/// 攻撃内容:分身巨大弾を設定するか
	/// </summary>
	/// <returns></returns>
	bool AttackTypeAvatarGiantBulletSelect();

	/// <summary>
	/// 攻撃状態
	/// </summary>
	bool AttackModeCount();

	/// <summary>
	/// 攻撃内容;追従
	/// </summary>
	/// <returns></returns>
	bool AttackTypeTracking();

	/// <summary>
	/// 攻撃内容:超巨大弾
	/// </summary>
	/// <returns></returns>
	bool AttackTypeSuperGiantBullet();

	/// <summary>
	/// 攻撃内容:回転
	/// </summary>
	/// <returns></returns>
	bool AttackTypeRotate();

	/// <summary>
	/// 攻撃内容:分身体当たり
	/// </summary>
	/// <returns></returns>
	bool AttackTypeAvatarBodyBlow();

	/// <summary>
	/// 攻撃内容:分身ガトリング砲
	/// </summary>
	/// <returns></returns>
	bool AttackTypeAvatarGatling();

	/// <summary>
	/// 攻撃内容:分身巨大弾
	/// </summary>
	/// <returns></returns>
	bool AttackTypeAvatarGiantBullet();

	/// <summary>
	/// 攻撃状態用の角度に本体回転
	/// </summary>
	bool AttackModeMainBodyRota();

	/// <summary>
	/// 攻撃状態用の角度に分身回転
	/// </summary>
	bool AttackModeAvatarRota();

	/// <summary>
	/// 待機状態
	/// </summary>
	bool WaitModeCount();

	/// <summary>
	/// 基準位置に戻る
	/// </summary>
	/// <returns></returns>
	bool ReturnBasePosition();

	/// <summary>
	/// 待機状態用の角度に本体回転
	/// </summary>
	bool WaitModeMainBodyRota();

	/// <summary>
	/// 待機状態用の角度に分身回転
	/// </summary>
	bool WaitModeAvatarRota();

	/// <summary>
	/// 死亡状態
	/// </summary>
	bool DeadMode();

	//getter
	const Phase& GetPhase() { return this->phase; }
	BossMainBody* GetMainBody() { return mainBody.get(); }
	const std::list<std::unique_ptr<BossAvatar>>& GetAvatars() { return avatars; };
	const bool GetIsDead() { return isDead; }

private: //メンバ関数
	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="damageNum">ダメージ量</param>
	void Damage(const int damageNum);

	/// <summary>
	/// 分身が全滅したかチェックする
	/// </summary>
	void CheckAllAvatarDead();

	/// <summary>
	/// 攻撃状態を終了するので必要な情報をセット
	/// 基準位置に戻るときの出発座標を記録する、弾発射状態解除
	/// </summary>
	void AttackEnd();


private: //静的メンバ変数
	//プレイヤー自機
	static Player* player;
	//攻撃状態時間
	static const float attackModeTime;
	//待機状態時間
	static const float waitModeTime;
	//モードチェンジ回転に要する時間
	static const float changeModeTime;
	//基準位置に戻るために要する時間
	static const float returnBasePositionTime;

private: //メンバ変数
	//ボスの行動遷移ビヘイビアツリー
	std::unique_ptr<BossBehaviorTree> behaviorTree;
	//本体
	std::unique_ptr<BossMainBody> mainBody;
	//分身
	std::list<std::unique_ptr<BossAvatar>> avatars;
	//本体が攻撃する状態か
	bool isMainBodyAttackMode = false;
	//行動
	Phase phase = Phase::Fall;
	//降下状態時間タイマー
	int32_t fallModeTimer = 0;
	//攻撃内容
	AttackType attackType = AttackType::None;
	//1つ前に発動した攻撃内容
	AttackType preAttackType = AttackType::None;
	//攻撃状態時間タイマー
	int32_t attackModeTimer = 0;
	//待機状態時間タイマー
	int32_t waitModeTimer = 0;
	//死亡状態時間タイマー
	int32_t deadModeTimer = 0;
	//体力
	int HP = 0;
	//HPバー
	std::unique_ptr<BossHPBar> hpBar;
	//HPバーフレーム
	std::unique_ptr<BossHPFrame> hpFrame;
	//死亡フラグ
	bool isDead = false;
};
