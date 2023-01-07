#pragma once
#include "ObjObject3d.h"

//ステージシーンの前方宣言
class BaseStageScene;

/// <summary>
/// ギャラクシー(船首)
/// </summary>
class GalaxyBow : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ギャラクシー(船首)</returns>
	static GalaxyBow* Create(ObjObject3d* parent, const Vector3& position);

	//getter
	static const int GetMaxHP() { return GalaxyBow::maxHP; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { GalaxyBow::stageScene = stageScene; }
	static void SetBowModel(ObjModel* model) { GalaxyBow::bowModel = model; }
	static void SetBowDeadModel(ObjModel* model) { GalaxyBow::bowDeadModel = model; }
	static void SetBulletModel(ObjModel* model) { GalaxyBow::bulletModel = model; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Update() override;

	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="attackPower">攻撃力</param>
	void Damage(int attackPower, const Vector3& collisionPos);

	/// <summary>
	/// 弾発射座標を更新
	/// </summary>
	void UpdateBulletShotPos();

	/// <summary>
	/// 攻撃状態開始処理
	/// </summary>
	void AttackModeStart();

	//getter	
	const bool GetIsDead() { return isDead; }
	const int GetDamageNum() { return damageNum; }
	const bool GetIsDamageTrigger() { return isDamageTrigger; }
	const bool GetIsAttack() { return isAttack; }

protected: //メンバ関数
	/// <summary>
	/// 通常弾発射
	/// </summary>
	void Fire(const float scale, const float bulletSpeed);

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


protected: //静的メンバ変数
	//ステージシーン
	static BaseStageScene* stageScene;
	//最大体力
	static const int maxHP = 40;
	//船首のモデル
	static ObjModel* bowModel;
	//死亡状態のモデル
	static ObjModel* bowDeadModel;
	//敵弾のモデル
	static ObjModel* bulletModel;
	//通常サイズ
	static const Vector3 normalSize;
	//ダメージ状態のサイズ
	static const Vector3 damageSize;
	//ダメージ状態の色
	static const XMFLOAT4 damageColor;

protected: //メンバ変数
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
};