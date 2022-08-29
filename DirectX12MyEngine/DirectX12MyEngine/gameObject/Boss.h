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
		None,	//未設定
		A,
		A2,
		B,
		C,
		D,
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス</returns>
	static Boss* Create(ObjModel* mainBodyModel, ObjModel* avatarModel, const Vector3& position);

	//setter
	static void SetPlayer(Player* player) { Boss::player = player; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(ObjModel* mainBodyModel, ObjModel* avatarModel, const Vector3& position);

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
	/// 降下
	/// </summary>
	bool Fall();

	/// <summary>
	/// 攻撃内容設定を開始するか
	/// </summary>
	/// <returns></returns>
	bool AttackTypeSelectStart();

	/// <summary>
	/// 攻撃内容Aを設定するか
	/// </summary>
	/// <returns></returns>
	bool AttackTypeASelect();

	/// <summary>
	/// 攻撃内容A2を設定するか
	/// </summary>
	/// <returns></returns>
	bool AttackTypeA2Select();

	/// <summary>
	/// 攻撃内容Bを設定するか
	/// </summary>
	/// <returns></returns>
	bool AttackTypeBSelect();

	/// <summary>
	/// 攻撃内容Cを設定するか
	/// </summary>
	/// <returns></returns>
	bool AttackTypeCSelect();

	/// <summary>
	/// 攻撃内容Dを設定するか
	/// </summary>
	/// <returns></returns>
	bool AttackTypeDSelect();

	/// <summary>
	/// 攻撃状態
	/// </summary>
	bool AttackModeCount();

	/// <summary>
	/// 攻撃内容A
	/// </summary>
	/// <returns></returns>
	bool AttackTypeA();

	/// <summary>
	/// 攻撃内容A2
	/// </summary>
	/// <returns></returns>
	bool AttackTypeA2();

	/// <summary>
	/// 攻撃内容B
	/// </summary>
	/// <returns></returns>
	bool AttackTypeB();

	/// <summary>
	/// 攻撃内容C
	/// </summary>
	/// <returns></returns>
	bool AttackTypeC();

	/// <summary>
	/// 攻撃内容D
	/// </summary>
	/// <returns></returns>
	bool AttackTypeD();

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
	/// 固定位置に戻る
	/// </summary>
	/// <returns></returns>
	bool ReturnFixedPosition();

	/// <summary>
	/// 待機状態用の角度に本体回転
	/// </summary>
	bool WaitModeMainBodyRota();

	/// <summary>
	/// 待機状態用の角度に分身回転
	/// </summary>
	bool WaitModeAvatarRota();

	//getter
	const Phase& GetPhase() { return this->phase; }
	BossMainBody* GetMainBody() { return mainBody.get(); }
	const std::list<std::unique_ptr<BossAvatar>>& GetAvatars() { return avatars; };

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
	/// 固定位置に戻るときの出発座標を記録する、弾発射状態解除
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
	//固定位置に戻るために要する時間
	static const float returnFixedPositionTime;

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
	//降下する時間タイマー
	int32_t fallTimer = 0;
	//攻撃内容
	AttackType attackType = AttackType::None;
	//1つ前に発動した攻撃内容
	AttackType preAttackType = AttackType::None;
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
