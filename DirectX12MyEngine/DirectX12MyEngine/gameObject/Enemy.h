#pragma once
#include "ObjObject3d.h"
#include "EnemyBullet.h"
#include "Vector2.h"

//自機クラスを前方宣言
class Player;
//GameSceneの前方宣言
class GameScene;

/// <summary>
/// 敵
/// </summary>
class Enemy : public ObjObject3d
{
public: //enum
	enum EnemyType {
		Demo,		//デモ用敵
		Cannon,		//大砲
		Circular,	//円運動
		Fall,		//降下
		UpDown,		//上下移動
		ComeGo,		//到着出発
	};

public: //静的メンバ関数
	//setter
	static void SetPlayer(Player* player) { Enemy::player = player; }
	static void SetGameScene(GameScene* gameScene) { Enemy::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { Enemy::bulletModel = model; }

public: //メンバ関数

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	virtual void OnCollision();

	//getter
	Vector3 GetWorldPos();
	Vector2 GetScreenPos();
	bool GetIsDead() const { return isDead; }

protected: //メンバ関数
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// 画面手前まで行ったら削除する処理
	/// </summary>
	void FrontOfScreenDelete();

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
