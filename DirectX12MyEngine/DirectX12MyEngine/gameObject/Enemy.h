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
public: //静的メンバ関数
	//setter
	static void SetPlayer(Player* player) { Enemy::player = player; }
	static void SetGameScene(GameScene* gameScene) { Enemy::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { Enemy::bulletModel = model; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	virtual void OnCollision();

	//getter
	Vector3 GetWorldPos();
	bool GetIsDead() const { return isDead; }

protected: //メンバ関数
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

protected: //静的メンバ変数
	//プレイヤー自機
	static Player* player;
	//ゲームシーン
	static GameScene* gameScene;
	//敵弾のモデル
	static ObjModel* bulletModel;

protected: //メンバ変数
	//死亡フラグ
	bool isDead = false;
};
