#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include "Reticle.h"
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
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision();

	//getter
	const int GetHP() { return HP; }
	const bool GetIsDead() { return isDead; }
	Vector3 GetWorldPos();
	Reticle* GetReticle() { return reticle2.get(); }
	const bool GetIsChargeShotMode() { return isChargeShotMode; }

private: //メンバ関数
	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 直進弾発射
	/// </summary>
	void ShotStraightBullet();

	/// <summary>
	/// ホーミング弾発射
	/// </summary>
	void ShotHomingBullet();

private: //静的メンバ変数
	//ゲームシーン
	static GameScene* gameScene;
	//自機弾のモデル
	static ObjModel* bulletModel;
	//自機の回転限界
	static const Vector2 rotLimit;

private: //メンバ変数
	//体力
	int HP = 0;
	//死亡フラグ
	bool isDead = false;
	//レティクル
	std::unique_ptr<Reticle> reticle;
	std::unique_ptr<Reticle> reticle2;
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
