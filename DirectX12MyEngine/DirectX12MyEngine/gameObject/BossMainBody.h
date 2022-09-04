#pragma once
#include "ObjObject3d.h"

//GameSceneの前方宣言
class GameScene;

/// <summary>
/// ボス(本体)
/// </summary>
class BossMainBody : public ObjObject3d
{
private: //攻撃内容ごとのフェーズ
	enum class AttackTypeTrackingPhase {
		Lockon,
		Shot,
		Stay,
	};

	enum class AttackTypeSuperGiantBulletPhase {
		Move,
		ChargeShot,
		Stay,
	};

	enum class AttackTypeRotatePhase {
		Wait,
		Shot,
		Move,
		Stay,
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ボス(本体)</returns>
	static BossMainBody* Create(const Vector3& basePos);

	//getter
	static const int GetMaxHP() { return BossMainBody::maxHP; }

	//setter
	static void SetGameScene(GameScene* gameScene) { BossMainBody::gameScene = gameScene; }
	static void SetBossMainBodyModel(ObjModel* model) { BossMainBody::mainBodyModel = model; }
	static void SetBossMainBodySleepModel(ObjModel* model) { BossMainBody::mainBodySleepModel = model; }
	static void SetBulletModel(ObjModel* model) { BossMainBody::bulletModel = model; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Update() override;

	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="attackPower">攻撃力</param>
	void Damage(int attackPower);

	/// <summary>
	/// 降下状態
	/// </summary>
	/// <param name="time">イージング用(0～1)の数値</param>
	void FallMode(const float time);

	/// <summary>
	/// 攻撃内容:追従
	/// </summary>
	void AttackTypeTracking(const Vector3& playerPosition);

	/// <summary>
	/// 攻撃内容:超巨大弾
	/// </summary>
	void AttackTypeSuperGiantBullet();

	/// <summary>
	/// 攻撃内容:回転
	/// </summary>
	void AttackTypeRotate();

	/// <summary>
	/// 攻撃内容:分身体当たり
	/// </summary>
	void AttackTypeAvatarBodyBlow();

	/// <summary>
	/// 攻撃内容:分身ガトリング砲
	/// </summary>
	void AttackTypeAvatarGatling(const Vector3& playerPosition);

	/// <summary>
	/// 攻撃状態に変更
	/// </summary>
	/// <param name="time">イージング用(0～1)の数値</param>
	void ChangeAttackMode(const float time);

	/// <summary>
	/// 待機状態に変更
	/// </summary>
	/// <param name="time">イージング用(0～1)の数値</param>
	void ChangeWaitMode(const float time);

	/// <summary>
	/// 基準位置に戻る
	/// </summary>
	/// <param name="time">イージング用(0～1)の数値</param>
	void ReturnBasePosition(const float time);

	/// <summary>
	/// 攻撃状態を終了するので必要な情報をセット
	/// 基準位置に戻るときの出発座標を記録する、弾発射状態解除
	/// </summary>
	void AttackEnd();

	/// <summary>
	/// 死亡状態
	/// </summary>
	void DeadMode();

	/// <summary>
	/// 寝ている状態のモデルから起きている状態のモデルに変更
	/// </summary>
	void ChangeModel();

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }
	const int GetDamageNum() { return damageNum; }

private: //メンバ関数
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire(const float scale, const float bulletSpeed);

	/// <summary>
	/// ダメージを喰らった状態の色にする
	/// </summary>
	void DamageColorMode();

	/// <summary>
	/// 攻撃内容:追従のロックオン処理
	/// </summary>
	void AttackTypeTrackingLockon();

	/// <summary>
	/// 攻撃内容:追従の弾発射処理
	/// </summary>
	void AttackTypeTrackingShot();

	/// <summary>
	/// 攻撃内容:超巨大弾の移動処理
	/// </summary>
	void AttackTypeSuperGiantBulletMove();

	/// <summary>
	/// 攻撃内容:超巨大弾のチャージショット処理
	/// </summary>
	void AttackTypeSuperGiantBulletChargeShot();

	/// <summary>
	/// 攻撃内容:回転の待機処理
	/// </summary>
	void AttackTypeRotateWait();

	/// <summary>
	/// 攻撃内容:回転の弾発射処理
	/// </summary>
	void AttackTypeRotateShot();

	/// <summary>
	/// 攻撃内容:回転の移動処理
	/// </summary>
	void AttackTypeRotateMove();

	/// <summary>
	/// 待機処理
	/// </summary>
	void Stay();

private: //静的メンバ変数
	//ゲームシーン
	static GameScene* gameScene;
	//本体のモデル
	static ObjModel* mainBodyModel;
	//本体の寝ている状態のモデル
	static ObjModel* mainBodySleepModel;
	//敵弾のモデル
	static ObjModel* bulletModel;
	//体力
	static const int maxHP = 60;
	//攻撃状態のY軸回転
	static const float attackModeRotY;
	//待機状態のY軸回転
	static const float waitModeRotY;
	//攻撃内容:追従の行動遷移
	static void (BossMainBody::* attackTypeTrackingPhaseFuncTable[])();
	//攻撃内容:超巨大弾の行動遷移
	static void (BossMainBody::* attackTypeSuperGiantBulletPhaseFuncTable[])();
	//攻撃内容:回転の行動遷移
	static void (BossMainBody::* attackTypeRotatePhaseFuncTable[])();

private: //メンバ変数
	//停止する基準の座標
	Vector3 basePos;
	//体力
	int HP = maxHP;
	//喰らうダメージ量
	int damageNum;
	//ダメージ色フラグ
	bool isDamageColor = false;
	//ダメージ色にする時間タイマー
	int32_t damageColorTimer = 0;
	//死亡フラグ
	bool isDead = false;
	//弾発射タイマー
	int32_t fireTimer = 0;
	//基準位置に戻るときの出発座標
	Vector3 returnStartPos;
	//攻撃で使うタイマー
	int32_t attackTimer = 0;
	//攻撃内容:追従の行動
	AttackTypeTrackingPhase attackTrackingPhase = AttackTypeTrackingPhase::Lockon;
	//攻撃内容:超巨大弾の行動
	AttackTypeSuperGiantBulletPhase attackSuperGiantBulletPhase = AttackTypeSuperGiantBulletPhase::Move;
	//攻撃内容:回転の行動
	AttackTypeRotatePhase attackRotatePhase = AttackTypeRotatePhase::Wait;
	//攻撃内容:回転で使う攻撃回数
	int attackRotateShotCount = 0;
	//攻撃内容:回転で移動前座標
	Vector3 attackRotateMoveBeforePos;
	//攻撃内容:回転で移動後座標
	Vector3 attackRotateMpveAfterPos;
};
