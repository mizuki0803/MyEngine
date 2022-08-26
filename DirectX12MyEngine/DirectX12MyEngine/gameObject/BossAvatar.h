#pragma once
#include "ObjObject3d.h"

//GameSceneの前方宣言
class GameScene;

/// <summary>
/// ボス(分身)
/// </summary>
class BossAvatar : public ObjObject3d
{
protected:
	enum class AttackTypeBPhase {
		Lockon,
		Shot,
		Back,
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
	void Fire();

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
	/// 攻撃内容Bの待機処理
	/// </summary>
	void AttackTypeBStay();

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

protected: //メンバ変数
	//体力
	int HP = maxHP;
	//死亡フラグ
	bool isDead = false;
	//弾発射タイマー
	int32_t fireTimer = 0;
	//弾発射開始するか
	bool isFire = false;
	//ボス本体との親子関係上の定位置座標
	Vector3 fixedPos;
	//固定位置に戻るときの出発座標
	Vector3 returnStartPos;
	//攻撃内容Bの行動
	AttackTypeBPhase attackBPhase = AttackTypeBPhase::Lockon;
	//攻撃内容Bで飛ばす角度
	Vector3 attackBVelocity;
	//攻撃内容Bで使うタイマー
	int32_t attackBTimer = 0;
	//攻撃内容Bでロックオン対象になる座標
	Vector3 attackBLockonPos;
};
