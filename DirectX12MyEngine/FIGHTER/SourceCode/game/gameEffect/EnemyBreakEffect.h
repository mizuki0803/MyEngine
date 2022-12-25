#pragma once
#include "ObjObject3d.h"
#include "GameCamera.h"

/// <summary>
/// 敵破壊用エフェクト
/// </summary>
class EnemyBreakEffect : public ObjObject3d
{
public: // 静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>敵破壊用エフェクト</returns>
	static EnemyBreakEffect* Create(ObjModel* model, const Vector3& position, const Vector3& velocity, const Vector3& rotSpeed, const Vector3& scale);

	//setter
	static void SetIsGroundMode(bool isGroundMode) { EnemyBreakEffect::isGroundMode = isGroundMode; }
	static void SetIsGravityMode(bool isGravityMode) { EnemyBreakEffect::isGravityMode = isGravityMode; }
	static void SetGameCamera(GameCamera* gameCamera) { EnemyBreakEffect::gameCamera = gameCamera; }

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//getter
	bool GetIsDelete() const { return isDelete; }

private: //メンバ関数
	/// <summary>
	/// 移動させる
	/// </summary>
	void Move();

	/// <summary>
	/// 画面手前まで行ったら削除する処理
	/// </summary>
	void FrontOfScreenDelete();

protected: //静的メンバ変数
	//地面ありの行動を行うか
	static bool isGroundMode;
	//重力ありの行動を行うか
	static bool isGravityMode;
	//ゲームカメラ
	static GameCamera* gameCamera;

protected: //メンバ変数
	//速度
	Vector3 velocity;
	//回転の速さ
	Vector3 rotSpeed;
	//削除フラグ
	bool isDelete = false;
};