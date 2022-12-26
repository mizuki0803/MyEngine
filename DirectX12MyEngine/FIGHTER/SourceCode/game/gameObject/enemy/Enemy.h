#pragma once
#include "ObjObject3d.h"
#include "Vector2.h"
#include "EnemyBullet.h"
#include "EnemyBreakEffect.h"

//自機クラスを前方宣言
class BasePlayer;
//ステージシーンの前方宣言
class BaseStageScene;

/// <summary>
/// 敵
/// </summary>
class Enemy : public ObjObject3d
{
public: //enum
	enum EnemyType {
		None,		//未設定
		Cannon,		//大砲
		Circular,	//円運動
		Fall,		//降下
		UpDown,		//上下移動
		ComeGo,		//到着出発
		Meteorite,	//破壊可能隕石
	};

public: //静的メンバ関数
	//setter
	static void SetPlayer(BasePlayer* player) { Enemy::player = player; }
	static void SetStageScene(BaseStageScene* stageScene) { Enemy::stageScene = stageScene; }
	static void SetBulletModel(ObjModel* model) { Enemy::bulletModel = model; }
	static void SetIsGroundMode(bool isGroundMode) { Enemy::isGroundMode = isGroundMode; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	virtual void OnCollision(const int damageNum);

	//getter
	Vector3 GetWorldPos();
	Vector2 GetScreenPos();
	bool GetIsCollisionFrame() const { return isCollisionFrame; }
	bool GetIsDead() const { return isDead; }
	bool GetIsDelete() const { return isDelete; }
	bool GetIsHitDelete() const { return isHitDelete; }

protected: //メンバ関数
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// ダメージ状態の処理
	/// </summary>
	void DamageMode();

	/// <summary>
	/// ダメージ状態で大きくしたサイズを戻していく処理
	/// </summary>
	void DamageSizeReturn();

	/// <summary>
	/// ダメージを喰らった状態の色にする
	/// </summary>
	void DamageColorMode();

	/// <summary>
	/// 死亡後の黒煙エフェクト
	/// </summary>
	/// <param name="size">エフェクトの大きさ</param>
	/// <param name="smokeStartTime">死亡してから黒煙を出すまでの時間</param>
	void DeadSmokeEffect(const float size, const int smokeStartTime = 30);

	/// <summary>
	/// 破壊
	/// </summary>
	virtual void Break();

	/// <summary>
	/// 破壊エフェクト追加
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="velocity">速度</param>
	/// <param name="rotSpeed">回転の速さ</param>
	/// <param name="scale">大きさ</param>
	void BreakEffect(ObjModel* model, const Vector3& velocity, const Vector3& rotSpeed, const Vector3& scale);

	/// <summary>
	/// 画面手前まで行ったら削除する処理
	/// </summary>
	void FrontOfScreenDelete();

protected: //静的メンバ変数
	//プレイヤー自機
	static BasePlayer* player;
	//ステージシーン
	static BaseStageScene* stageScene;
	//敵弾のモデル
	static ObjModel* bulletModel;
	//地面ありの行動を行うか
	static bool isGroundMode;
	//ダメージ状態の色
	static const XMFLOAT4 damageColor;

protected: //メンバ変数
	//HP
	int HP = 0;
	//通常サイズ
	Vector3 normalSize;
	//ダメージ状態のサイズ
	Vector3 damageSize;
	//ダメージ状態か
	bool isDamage = false;
	//ダメージ色か
	bool isDamageColor = false;
	//ダメージ状態タイマー
	int32_t damageTimer;
	//当たり判定が作用したフレームか
	bool isCollisionFrame = false;
	//死亡フラグ
	bool isDead = false;
	//死亡してからの時間タイマー
	int32_t deadTimer = 0;
	//削除フラグ
	bool isDelete = false;
	//被弾して削除か
	bool isHitDelete = false;
};
