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
	enum class AttackTypeAPhase {
		Lockon,
		Shot,
		Stay,
	};

	enum class AttackTypeA2Phase {
		Move,
		ChargeShot,
		Stay,
	};

	enum class AttackTypeA3Phase {
		Wait,
		Shot,
		Move,
		Stay,
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス(本体)</returns>
	static BossMainBody* Create(ObjModel* model, const Vector3& basePos);

	//getter
	static const int GetMaxHP() { return BossMainBody::maxHP; }

	//setter
	static void SetGameScene(GameScene* gameScene) { BossMainBody::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { BossMainBody::bulletModel = model; }

public: //メンバ関数
	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="damageNum">ダメージ量</param>
	void Damage(int damageNum);

	/// <summary>
	/// 降下
	/// </summary>
	/// <param name="time">イージング用(0～1)の数値</param>
	void Fall(const float time);

	/// <summary>
	/// 攻撃内容A
	/// </summary>
	void AttackTypeA(const Vector3& playerPosition);

	/// <summary>
	/// 攻撃内容A2
	/// </summary>
	void AttackTypeA2();

	/// <summary>
	/// 攻撃内容A3
	/// </summary>
	void AttackTypeA3();

	/// <summary>
	/// 攻撃内容B
	/// </summary>
	void AttackTypeB();

	/// <summary>
	/// 攻撃内容C
	/// </summary>
	void AttackTypeC(const Vector3& playerPosition);

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

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }

private: //メンバ関数
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire(const float scale, const float bulletSpeed);

	/// <summary>
	/// 攻撃内容Aのロックオン処理
	/// </summary>
	void AttackTypeALockon();

	/// <summary>
	/// 攻撃内容Aの弾発射処理
	/// </summary>
	void AttackTypeAShot();

	/// <summary>
	/// 攻撃内容A2の移動処理
	/// </summary>
	void AttackTypeA2Move();

	/// <summary>
	/// 攻撃内容A2のチャージショット処理
	/// </summary>
	void AttackTypeA2ChargeShot();

	/// <summary>
	/// 攻撃内容A3の待機処理
	/// </summary>
	void AttackTypeA3Wait();

	/// <summary>
	/// 攻撃内容A3の弾発射処理
	/// </summary>
	void AttackTypeA3Shot();

	/// <summary>
	/// 攻撃内容A3の移動処理
	/// </summary>
	void AttackTypeA3Move();

	/// <summary>
	/// 待機処理
	/// </summary>
	void Stay();

private: //静的メンバ変数
	//ゲームシーン
	static GameScene* gameScene;
	//敵弾のモデル
	static ObjModel* bulletModel;
	//体力
	static const int maxHP = 60;
	//攻撃状態のY軸回転
	static const float attackModeRotY;
	//待機状態のY軸回転
	static const float waitModeRotY;
	//攻撃内容Aの行動遷移
	static void (BossMainBody::* attackTypeAPhaseFuncTable[])();
	//攻撃内容A2の行動遷移
	static void (BossMainBody::* attackTypeA2PhaseFuncTable[])();
	//攻撃内容A3の行動遷移
	static void (BossMainBody::* attackTypeA3PhaseFuncTable[])();

private: //メンバ変数
	//停止する基準の座標
	Vector3 basePos;
	//体力
	int HP = maxHP;
	//死亡フラグ
	bool isDead = false;
	//弾発射タイマー
	int32_t fireTimer = 0;
	//基準位置に戻るときの出発座標
	Vector3 returnStartPos;
	//攻撃内容Aの行動
	AttackTypeAPhase attackAPhase = AttackTypeAPhase::Lockon;
	//攻撃内容Aで使うタイマー
	int32_t attackATimer = 0;
	//攻撃内容A2の行動
	AttackTypeA2Phase attackA2Phase = AttackTypeA2Phase::Move;
	//攻撃内容A2で使うタイマー
	int32_t attackA2Timer = 0;
	//攻撃内容A3の行動
	AttackTypeA3Phase attackA3Phase = AttackTypeA3Phase::Wait;
	//攻撃内容A3で使うタイマー
	int32_t attackA3Timer = 0;
	//攻撃内容A3で使う攻撃回数
	int attackA3ShotCount = 0;
	//攻撃内容A3で移動前座標
	Vector3 attackA3MoveBeforePos;
	//攻撃内容A3で移動後座標
	Vector3 attackA3MpveAfterPos;
	//攻撃内容Bで使うタイマー
	int32_t attackBTimer = 0;
};
