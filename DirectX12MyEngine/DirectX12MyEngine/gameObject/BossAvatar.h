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
	enum class AttackTypeBPhase {
		Lockon,
		Shot,
		Back,
		Stay,
	};

	enum class AttackTypeCPhase {
		MoveCenter,
		RotStart,
		Shot,
		RotEnd,
		Stay,
	};

	enum class AttackTypeDPhase {
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
	static void SetBulletModel(ObjModel* model) { BossAvatar::bulletModel = model; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="damageNum">ダメージ量</param>
	void Damage(int damageNum);

	/// <summary>
	/// 攻撃内容B
	/// </summary>
	void AttackTypeB(const Vector3& playerPosition);

	/// <summary>
	/// 攻撃内容C
	/// </summary>
	void AttackTypeC();

	/// <summary>
	/// 攻撃内容D
	/// </summary>
	void AttackTypeD();

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
	/// 固定位置に戻る
	/// </summary>
	/// <param name="time">イージング用(0～1)の数値</param>
	void ReturnFixedPosition(const float time);

	/// <summary>
	/// 攻撃状態を終了するので必要な情報をセット
	/// 固定位置に戻るときの出発座標を記録する、弾発射状態解除
	/// </summary>
	void AttackEnd();

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }

protected:
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire(const float scale, const float bulletSpeed);

	/// <summary>
	/// 攻撃内容Bのロックオン処理
	/// </summary>
	virtual void AttackTypeBLockon() = 0;

	/// <summary>
	/// 攻撃内容Bの飛ばす処理
	/// </summary>
	void AttackTypeBShot();

	/// <summary>
	/// 攻撃内容Bの元の位置に戻る処理
	/// </summary>
	void AttackTypeBBack();

	/// <summary>
	/// 攻撃内容Cの中心に移動させる処理
	/// </summary>
	virtual void AttackTypeCMoveCenter() = 0;

	/// <summary>
	/// 攻撃内容Cの回転開始処理
	/// </summary>
	void AttackTypeCRotStart();

	/// <summary>
	/// 攻撃内容Cの飛ばす処理
	/// </summary>
	void AttackTypeCShot();

	/// <summary>
	/// 攻撃内容Cの回転終了処理
	/// </summary>
	void AttackTypeCRotEnd();

	/// <summary>
	/// 攻撃内容Dの待機処理
	/// </summary>
	virtual void AttackTypeDWait() = 0;

	/// <summary>
	/// 攻撃内容Dの移動させる処理
	/// </summary>
	virtual void AttackTypeDMove() = 0;

	/// <summary>
	/// 攻撃内容Dのチャージショット処理
	/// </summary>
	void AttackTypeDChargeShot();

	/// <summary>
	/// 攻撃内容Dの反動で移動する処理
	/// </summary>
	void AttackTypeDRecoil();

	/// <summary>
	/// 待機処理
	/// </summary>
	void Stay();

protected: //静的メンバ変数
	//ゲームシーン
	static GameScene* gameScene;
	//敵弾のモデル
	static ObjModel* bulletModel;
	//体力
	static const int maxHP = 1;
	//攻撃状態のY軸回転
	static const float attackModeRotY;
	//待機状態のY軸回転
	static const float waitModeRotY;
	//攻撃内容Bの行動遷移
	static void (BossAvatar::* attackTypeBPhaseFuncTable[])();
	//攻撃内容Cの行動遷移
	static void (BossAvatar::* attackTypeCPhaseFuncTable[])();
	//攻撃内容Cで使う円の半径の長さ
	static const float attackCLength;
	//攻撃内容Dの行動遷移
	static void (BossAvatar::* attackTypeDPhaseFuncTable[])();

protected: //メンバ変数
	//体力
	int HP = maxHP;
	//死亡フラグ
	bool isDead = false;
	//弾発射タイマー
	int32_t fireTimer = 0;
	//ボス本体との親子関係上の定位置座標
	Vector3 fixedPos;
	//固定位置に戻るときの出発座標
	Vector3 returnStartPos;
	//攻撃内容Bの行動
	AttackTypeBPhase attackBPhase = AttackTypeBPhase::Lockon;
	//攻撃内容Bで使うタイマー
	int32_t attackBTimer = 0;
	//攻撃内容Bで飛ばす角度
	Vector3 attackBVelocity;
	//攻撃内容Bでロックオン対象になる座標
	Vector3 attackBLockonPos;
	//攻撃内容Cの行動
	AttackTypeCPhase attackCPhase = AttackTypeCPhase::MoveCenter;
	//攻撃内容Cで使うタイマー
	int32_t attackCTimer = 0;
	//攻撃内容Cで使う円運動用角度
	float attackCAngle;
	//攻撃内容Cで使う回転速度
	float attackCRotSpeed = 0;
	//攻撃内容Dの行動
	AttackTypeDPhase attackDPhase = AttackTypeDPhase::Wait;
	//攻撃内容Dで使うタイマー
	int32_t attackDTimer = 0;
	//攻撃内容Dで使う反動速度
	Vector3 attackDRecoilVelocity = { 0, 0, 2 };
	//攻撃内容Dで使う反動加速度
	Vector3 attackDRecoilAccel = { 0, 0, -0.05f };
};
