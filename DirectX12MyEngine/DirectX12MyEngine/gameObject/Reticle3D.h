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
	/// <returns>3Dレティクル</returns>
	static Reticle3D* Create(ObjModel* model);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	//bool Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	//void Draw() override;

	//setter
	void SetFollowMatWorld(const XMMATRIX& followMatWorld) { this->followMatWorld = followMatWorld; }

	//getter
	Vector3 GetWorldPos();

private:
	XMMATRIX followMatWorld = {};
};

