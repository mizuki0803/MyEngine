#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include "PlayerReticles.h"
#include "PlayerHPBar.h"
#include "PlayerHPFrame.h"
#include <memory>
#include <list>

//ゲームシーンの前方宣言
class GameScene;

/// <summary>
/// 自機
/// </summary>
class Player : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>自機</returns>
	static Player* Create(ObjModel* model);

	//getter
	static const Vector2& GetRotLimit() { return rotLimit; }

	//setter
	static void SetGameScene(GameScene* gameScene) { Player::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { Player::bulletModel = model; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 衝突時コールバック関数(敵などダメージを喰らう)
	/// </summary>
	void OnCollisionDamage(const Vector3& subjectPos);

	/// <summary>
	/// 衝突時コールバック関数(アイテムなど回復する)
	/// </summary>
	void OnCollisionHeal();

	//getter
	Vector3 GetWorldPos();
	const int GetHP() { return HP; }
	const bool GetIsDamage() { return isDamage; }
	const bool GetIsCrash() { return isCrash; }
	const int GetCrashBoundCount() { return crashBoundCount; }
	const bool GetIsDead() { return isDead; }
	const bool GetIsRoll() { return isRoll; }
	const Vector3& GetKnockbackVel() { return knockbackVel; }
	PlayerReticles* GetReticles() { return reticles.get(); }
	const bool GetIsChargeShotMode() { return isChargeShotMode; }

private: //メンバ関数
	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	void Damage();

	/// <summary>
	/// 墜落開始
	/// </summary>
	void CrashStart();

	/// <summary>
	/// 墜落
	/// </summary>
	void Crash();

	/// <summary>
	/// 回復
	/// </summary>
	void Heal();

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 移動
	/// </summary>
	void Roll();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 弾発射座標を更新
	/// </summary>
	void UpdateBulletShotPos();

	/// <summary>
	/// 直進弾発射
	/// </summary>
	void ShotStraightBullet();

	/// <summary>
	/// ホーミング弾発射
	/// </summary>
	void ShotHomingBullet();

	/// <summary>
	/// ノックバック情報をセット
	/// </summary>
	void SetKnockback(const Vector3& subjectPos);

	/// <summary>
	/// ノックバック
	/// </summary>
	void Knockback();

private: //静的メンバ変数
	//ゲームシーン
	static GameScene* gameScene;
	//自機弾のモデル
	static ObjModel* bulletModel;
	//ホーミング弾の大きさ
	static const float homingBulletSize;
	//自機の回転限界
	static const Vector2 rotLimit;
	//最大体力
	static const int maxHP = 101;

private: //メンバ変数
	//体力
	int HP = maxHP;
	//HPバー
	std::unique_ptr<PlayerHPBar> hpBar;
	//HPバーフレーム
	std::unique_ptr<PlayerHPFrame> hpFrame;
	//ダメージフラグ
	bool isDamage = false;
	//墜落中か
	bool isCrash = false;
	//墜落バウンド回数
	int crashBoundCount = 0;
	//墜落速度
	Vector3 crashVel = { 0, 0, 0.2f };
	//死亡フラグ
	bool isDead = false;
	//緊急回避中か
	bool isRoll = false;
	//緊急回避用タイマー
	int32_t rollTimer = 0;
	//緊急回避開始時のZ軸角度
	float rollStartRot = 0;
	//緊急回避終了時のZ軸角度
	float rollEndRot = 0;
	//ノックバック用タイマー
	int32_t knockbackTimer = 0;
	//ノックバック方向
	Vector3 knockbackVec;
	//ノックバック速度
	Vector3 knockbackVel;
	//レティクル
	std::unique_ptr<PlayerReticles> reticles;
	//弾発射座標
	Vector3 bulletShotPos;
	//直進弾の発射待機中か
	bool isStraightShotWait = false;
	//直進弾の発射待機タイマー
	int32_t straightShotWaitTimer = 0;
	//チャージショット中か
	bool isChargeShotMode = false;
	//チャージした時間
	int32_t chargeTimer = 0;
	//z軸ゆらゆら回転が右回転か
	bool isRotZRight = true;
	//z軸ゆらゆら回転用
	float swayZ = 0.0f;
};
