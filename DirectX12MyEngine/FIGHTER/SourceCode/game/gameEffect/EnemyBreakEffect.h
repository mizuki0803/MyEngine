#pragma once
#include "ObjObject3d.h"

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

protected: //メンバ変数
	//速度
	Vector3 velocity;
	//回転の速さ
	Vector3 rotSpeed;
	//削除フラグ
	bool isDelete = false;
};