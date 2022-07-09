#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 自機弾
/// </summary>
class PlayerBullet : public ObjObject3d
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>自機弾</returns>
	static PlayerBullet* Create(ObjModel* model, XMFLOAT3 position);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
};