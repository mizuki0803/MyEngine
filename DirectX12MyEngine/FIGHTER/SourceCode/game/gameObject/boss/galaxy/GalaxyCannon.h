#pragma once
#include "ObjObject3d.h"

//ステージシーンの前方宣言
class BaseStageScene;

/// <summary>
/// ギャラクシー(大砲)
/// </summary>
class GalaxyCannon : public ObjObject3d
{
private: //攻撃内容のフェーズ
	//速射
	enum class AttackTypeRapidFirePhase {
		StartWait,//開始時の待機
		Charge,	//溜め
		Shot,	//発射
		Return,	//戻す
		Wait,	//待機
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="parent">親オブジェクト</param> 
	/// <param name="basePos">基準座標</param>
	/// <returns>ギャラクシー(大砲)</returns>
	static GalaxyCannon* Create(ObjObject3d* parent, const Vector3& basePos);

	//getter
	static const int GetMaxHP() { return GalaxyCannon::maxHP; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { GalaxyCannon::stageScene = stageScene; }
	static void SetCannonModel(ObjModel* model) { GalaxyCannon::cannonModel = model; }
	static void SetCannonDeadModel(ObjModel* model) { GalaxyCannon::cannonDeadModel = model; }
	static void SetBulletModel(ObjModel* model) { GalaxyCannon::bulletModel = model; }
	static void SetBreakModel(int modelNum, ObjModel* model);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Update() override;

	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="attackPower">攻撃力</param>
	/// <param name="collisionPos">衝突座標</param>
	void Damage(int attackPower, const Vector3& collisionPos);

	/// <summary>
	/// 弾発射座標を更新
	/// </summary>
	void UpdateBulletShotPos();

	/// <summary>
	/// 攻撃状態開始処理
	/// </summary>
	void AttackModeStart();

	/// <summary>
	/// 攻撃内容:速射
	/// </summary>
	void AttackTypeRapidFire();

	//getter	
	const bool GetIsDead() { return isDead; }
	const int GetDamageNum() { return damageNum; }
	const bool GetIsDamageTrigger() { return isDamageTrigger; }
	const bool GetIsAttack() { return isAttack; }

	//setter
	void SetRapidFireStartWaitTime(const int startWaitTime) { rapidFireStartWaitTime = startWaitTime; }

protected: //メンバ関数
	/// <summary>
	/// 通常弾発射
	/// </summary>
	void Fire(const float scale, const float bulletSpeed);

	/// <summary>
	/// チャージ弾発射
	/// </summary>
	void ChargeBulletFire(const float scale, const float bulletSpeed);

	/// <summary>
	/// ダメージ状態の処理
	/// </summary>
	void DamageMode();

	/// <summary>
	/// ダメージ状態で大きくしたサイズを戻していく処理
	/// </summary>
	void DamageSizeReturn();

	/// <summary>
	/// ダメージ爆発
	/// </summary>
	void DamageExplosion(const Vector3& collisionPos);

	/// <summary>
	/// ダメージを喰らった状態の色をチカチカさせる処理
	/// </summary>
	void DamageColorChange();

	/// <summary>
	/// 攻撃内容:速射の開始時待機処理
	/// </summary>
	void AttackTypeRapidFireStartWait();

	/// <summary>
	/// 攻撃内容:速射の溜め処理
	/// </summary>
	void AttackTypeRapidFireCharge();

	/// <summary>
	/// 攻撃内容:速射の発射処理
	/// </summary>
	void AttackTypeRapidFireShot();

	/// <summary>
	/// 攻撃内容:速射の戻す処理
	/// </summary>
	void AttackTypeRapidFireReturn();

	/// <summary>
	/// 攻撃内容:速射の待機処理
	/// </summary>
	void AttackTypeRapidFireWait();

	/// <summary>
	/// 破壊
	/// </summary>
	void Break();


protected: //静的メンバ変数
	//ステージシーン
	static BaseStageScene* stageScene;
	//最大体力
	static const int maxHP = 6;
	//大砲のモデル
	static ObjModel* cannonModel;
	//死亡状態のモデル
	static ObjModel* cannonDeadModel;
	//敵弾のモデル
	static ObjModel* bulletModel;
	//破壊時に出すモデル
	static std::array<ObjModel*, 5> breakModels;
	//通常サイズ
	static const Vector3 normalSize;
	//ダメージ状態のサイズ
	static const Vector3 damageSize;
	//ダメージ状態の色
	static const XMFLOAT4 damageColor;
	//攻撃内容:速射の行動遷移
	static void (GalaxyCannon::* attackTypeRapidFirePhaseFuncTable[])();

protected: //メンバ変数
	//基準座標
	Vector3 basePos;
	//体力
	int HP = maxHP;
	//死亡フラグ
	bool isDead = false;
	//喰らうダメージ量
	int damageNum = 0;
	//ダメージフラグ
	bool isDamage = false;
	//ダメージを喰らった瞬間か
	bool isDamageTrigger = false;
	//ダメージ状態タイマー
	int32_t damageTimer = 0;
	//ダメージ色か
	bool isDamageColor = false;
	//弾発射タイマー
	int32_t fireTimer = 0;
	//弾発射座標
	Vector3 bulletShotPos;
	//攻撃中か
	bool isAttack = false;
	//攻撃で使うタイマー
	int32_t attackTimer = 0;
	//攻撃内容:速射の行動
	AttackTypeRapidFirePhase attackTypeRapidFirePhase = AttackTypeRapidFirePhase::StartWait;
	//攻撃内容:速射の開始時待機フレーム数
	int rapidFireStartWaitTime = 0;
	//攻撃内容:速射の発射回数
	int rapidFireCount = 0;
};