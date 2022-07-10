#pragma once
#include "ObjObject3d.h"

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
	static Enemy* Create(ObjModel* model, const XMFLOAT3& position, const XMFLOAT3& velocity);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 移動
	/// </summary>
	void Move();

private: //メンバ変数
	//速度
	XMFLOAT3 velocity;
	//行動フェーズ
	Phase phase = Phase::Previous;
};
