#pragma once
#include "ObjObject3d.h"
#include "EnemyBullet.h"

//自機クラスを前方宣言
class Player;

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
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 前進フェーズ初期化
	/// </summary>
	void PreviousPhaseInit();

	//setter
	void SetPlayer(Player* player) { this->player = player; }

	//getter
	Vector3 GetWorldPos();
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return enemyBullets; };

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

private: //メンバ変数
	//速度
	Vector3 velocity;
	//行動フェーズ
	Phase phase = Phase::Previous;
	//敵弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;
	//発射タイマー
	int32_t fireTimer = 0;
	//プレイヤー自機
	Player* player = nullptr;
};
