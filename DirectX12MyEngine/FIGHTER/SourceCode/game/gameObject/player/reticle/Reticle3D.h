#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 3Dレティクル
/// </summary>
class Reticle3D : public ObjObject3d
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="distance">発射対象からの距離</param>
	/// <returns>3Dレティクル</returns>
	static Reticle3D* Create(ObjModel* model, float distance);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//setter
	void SetFollowMatWorld(const XMMATRIX& followMatWorld) { this->followMatWorld = followMatWorld; }

private:
	//追従対象のワールド行列
	XMMATRIX followMatWorld = {};
	//追従対象との距離
	float distance = 0;
};

