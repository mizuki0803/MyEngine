#pragma once
#include "ObjObject3d.h"
#include "EnemyBullet.h"

//自機クラスを前方宣言
class Player;
//GameSceneの前方宣言
class GameScene;

/// <summary>
/// 敵
/// </summary>
class Enemy : public ObjObject3d
{
public:
	//行動フェーズ
	enum class Phase {
		Previous,	//前進
		Back,		//後退
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>敵</returns>
	static Enemy* Create(ObjModel* model, const Vector3& position, const Vector3& velocity);

	//setter
	static void SetPlayer(Player* player) { Enemy::player = player; }
	static void SetGameScene(GameScene* gameScene) { Enemy::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { Enemy::bulletModel = model; }

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
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 前進フェーズ初期化
	/// </summary>
	void PreviousPhaseInit();

	//getter
	Vector3 GetWorldPos();

private: //メンバ関数
	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

private: //静的メンバ変数
	//発射間隔
	static const int fireInterval = 60;
	//プレイヤー自機
	static Player* player;
	//ゲームシーン
	static GameScene* gameScene;
	//敵弾のモデル
	static ObjModel* bulletModel;

private: //メンバ変数
	//速度
	Vector3 velocity;
	//行動フェーズ
	Phase phase = Phase::Previous;
	//発射タイマー
	int32_t fireTimer = 0;
};
