#pragma once
#include "ObjObject3d.h"

//GameSceneの前方宣言
class GameScene;

/// <summary>
/// ボス(分身)
/// </summary>
class BossAvatar : public ObjObject3d
{
protected: //攻撃内容ごとのフェーズ
	enum class AttackTypeAvatarBodyBlowPhase {
		Lockon,
		Shot,
		Back,
		Stay,
	};

	enum class AttackTypeAvatarGatlingPhase {
		MoveCenter,
		RotStart,
		Shot,
		RotEnd,
		Stay,
	};

	enum class AttackTypeAvatarGiantBulletPhase {
		Wait,
		Move,
		ChargeShot,
		Recoil,
		Stay,
	};

public: //静的メンバ関数
	//getter
	static const int GetMaxHP() { return BossAvatar::maxHP; }

	//setter
	static void SetGameScene(GameScene* gameScene) { BossAvatar::gameScene = gameScene; }
	static void SetAvatarModel(ObjModel* model) { BossAvatar::avatarModel = model; }
	static void SetAvatarSleepModel(ObjModel* model) { BossAvatar::avatarSleepModel = model; }
	static void SetBulletModel(ObjModel* model) { BossAvatar::bulletModel = model; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

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
	/// 攻撃内容:分身体当たり
	/// </summary>
	void AttackTypeAvatarBodyBlow(const Vector3& playerPosition);

	/// <summary>
	/// 攻撃内容:分身ガトリング砲
	/// </summary>
	void AttackTypeAvatarGatling();

	/// <summary>
	/// 攻撃内容:分身巨大弾
	/// </summary>
	void AttackTypeAvatarGiantBullet();

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
	/// 寝ている状態のモデルから起きている状態のモデルに変更
	/// </summary>
	void ChangeModel();

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }
	const int GetDamageNum() { return damageNum; }

protected:
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire(const float scale, const float bulletSpeed);

	/// <summary>
	/// ダメージを喰らった状態の色にする
	/// </summary>
	void DamageColorMode();

	/// <summary>
	/// 攻撃内容:分身体当たりのロックオン処理
	/// </summary>
	virtual void AttackTypeAvatarBodyBlowLockon() = 0;

	/// <summary>
	/// 攻撃内容:分身体当たりの飛ばす処理
	/// </summary>
	void AttackTypeAvatarBodyBlowShot();

	/// <summary>
	/// 攻撃内容:分身体当たりの元の位置に戻る処理
	/// </summary>
	void AttackTypeAvatarBodyBlowBack();

	/// <summary>
	/// 攻撃内容:分身ガトリング砲の中心に移動させる処理
	/// </summary>
	virtual void AttackTypeAvatarGatlingMoveCenter() = 0;

	/// <summary>
	/// 攻撃内容:分身ガトリング砲の回転開始処理
	/// </summary>
	void AttackTypeAvatarGatlingRotStart();

	/// <summary>
	/// 攻撃内容:分身ガトリング砲の飛ばす処理
	/// </summary>
	void AttackTypeAvatarGatlingShot();

	/// <summary>
	/// 攻撃内容:分身ガトリング砲の回転終了処理
	/// </summary>
	void AttackTypeAvatarGatlingRotEnd();

	/// <summary>
	/// 攻撃内容:分身巨大弾の待機処理
	/// </summary>
	virtual void AttackTypeAvatarGiantBulletWait() = 0;

	/// <summary>
	/// 攻撃内容:分身巨大弾の移動させる処理
	/// </summary>
	virtual void AttackTypeAvatarGiantBulletMove() = 0;

	/// <summary>
	/// 攻撃内容:分身巨大弾のチャージショット処理
	/// </summary>
	void AttackTypeAvatarGiantBulletChargeShot();

	/// <summary>
	/// 攻撃内容:分身巨大弾の反動で移動する処理
	/// </summary>
	void AttackTypeAvatarGiantBulletRecoil();

	/// <summary>
	/// 待機処理
	/// </summary>
	void Stay();

protected: //静的メンバ変数
	//ゲームシーン
	static GameScene* gameScene;
	//分身のモデル
	static ObjModel* avatarModel;
	//分身の寝ている状態のモデル
	static ObjModel* avatarSleepModel;
	//敵弾のモデル
	static ObjModel* bulletModel;
	//体力
	static const int maxHP = 15;
	//攻撃状態のY軸回転
	static const float attackModeRotY;
	//待機状態のY軸回転
	static const float waitModeRotY;
	//攻撃内容:分身体当たりの行動遷移
	static void (BossAvatar::* attackTypeAvatarBodyBlowPhaseFuncTable[])();
	//攻撃内容:分身ガトリング砲の行動遷移
	static void (BossAvatar::* attackTypeAvatarGatlingPhaseFuncTable[])();
	//攻撃内容:分身ガトリング砲で使う円の半径の長さ
	static const float attackAvatarGatlingLength;
	//攻撃内容:分身巨大弾の行動遷移
	static void (BossAvatar::* attackTypeAvatarGiantBulletPhaseFuncTable[])();

protected: //メンバ変数
	//体力
	int HP = maxHP;
	//死亡フラグ
	bool isDead = false;
	//喰らうダメージ量
	int damageNum;
	//ダメージ色フラグ
	bool isDamageColor = false;
	//ダメージ色にする時間タイマー
	int32_t damageColorTimer = 0;
	//弾発射タイマー
	int32_t fireTimer = 0;
	//ボス本体との親子関係上の基準座標
	Vector3 basePos;
	//基準位置に戻るときの出発座標
	Vector3 returnStartPos;
	//攻撃で使うタイマー
	int32_t attackTimer = 0;
	//攻撃内容:分身体当たりの行動
	AttackTypeAvatarBodyBlowPhase attackAvatarBodyBlowPhase = AttackTypeAvatarBodyBlowPhase::Lockon;
	//攻撃内容:分身体当たりで飛ばす角度
	Vector3 attackAvatarBodyBlowVelocity;
	//攻撃内容:分身体当たりでロックオン対象になる座標
	Vector3 attackAvatarBodyBlowLockonPos;
	//攻撃内容:分身ガトリング砲の行動
	AttackTypeAvatarGatlingPhase attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::MoveCenter;
	//攻撃内容:分身ガトリング砲で使う円運動用角度
	float attackAvatarGatlingAngle;
	//攻撃内容:分身ガトリング砲で使う回転速度
	float attackAvatarGatlingRotSpeed = 0;
	//攻撃内容:分身巨大弾の行動
	AttackTypeAvatarGiantBulletPhase attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Wait;
	//攻撃内容:分身巨大弾で使う反動速度
	Vector3 attackAvatarGiantBulletRecoilVelocity = { 0, 0, 2 };
	//攻撃内容:分身巨大弾で使う反動加速度
	Vector3 attackAvatarGiantBulletRecoilAccel = { 0, 0, -0.05f };
};
