#pragma once
#include "BossMainBody.h"
#include "BossAvatar.h"
#include "BossNameUI.h"
#include "BossHPUI.h"
#include "BossBehaviorTree.h"

//自機クラスを前方宣言
class BasePlayer;

/// <summary>
/// ボス
/// </summary>
class Boss
{
public:
	//行動フェーズ
	enum class Phase {
		Appear,	//登場
		Attack,	//攻撃
		Wait,	//待機
		Dead,	//死亡
	};

	//攻撃種類
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
	static void SetPlayer(BasePlayer* player) { Boss::player = player; }

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
	/// 影用光源ライトから見た視点での描画
	/// </summary>
	void DrawLightCameraView();

	/// <summary>
	/// 頭上からの影用光源ライトから見た視点での描画
	/// </summary>
	void DrawTopLightCameraView();

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 衝突時コールバック関数(本体)
	/// </summary>
	/// <param name="bossAvatars">衝突した分身</param>
	/// <param name="damageNum">ダメージ量</param>
	void OnCollisionMainBody(const int damageNum, const Vector3& collisionPos, const Vector3& subjectVel);

	/// <summary>
	/// 衝突時コールバック関数(分身)
	/// </summary>
	/// <param name="avatars">衝突した分身</param>
	/// <param name="damageNum">ダメージ量</param>
	void OnCollisionAvatar(BossAvatar* avatar, const int damageNum, const Vector3& collisionPos, const Vector3& subjectVel);

	/// <summary>
	/// 登場状態
	/// </summary>
	bool AppearModeCount();

	/// <summary>
	/// 登場降下
	/// </summary>
	bool AppearFall();

	/// <summary>
	/// 攻撃状態
	/// </summary>
	bool AttackModeCount();

	/// <summary>
	/// 弾発射座標を更新
	/// </summary>
	bool UpdateBulletShotPos();

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
	/// 死亡爆発
	/// </summary>
	bool DeadExplosion();

	/// <summary>
	/// 死亡落下
	/// </summary>
	bool DeadFall();

	//getter
	const Phase& GetPhase() { return this->phase; }
	BossMainBody* GetMainBody() { return mainBody.get(); }
	const std::list<std::unique_ptr<BossAvatar>>& GetAvatars() { return avatars; };
	const bool GetIsDead() { return isDead; }
	const bool GetIsDelete() { return isDelete; }

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

	/// <summary>
	/// 基準位置に戻るときの出発座標を記録する
	/// </summary>
	void SetReturnBasePosition();

private: //静的メンバ変数
	//プレイヤー自機
	static BasePlayer* player;
	//登場状態時間
	static const float appearModeTime;
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
	Phase phase = Phase::Appear;
	//登場状態時間タイマー
	int32_t appearModeTimer = 0;
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
	//爆発演出発生タイマー
	int32_t explosionTimer = 0;
	//爆発発生回数
	int32_t explosionCount = 0;
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
