#pragma once
#include "Sprite.h"
#include "Reticle3D.h"

/// <summary>
/// 2Dレティクル
/// </summary>
class Reticle2D : public Sprite
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>レティクル2D</returns>
	static Reticle2D* Create(UINT texNumber);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	//void Draw() override;

	//setter
	void SetReticle(Reticle3D* reticle) { this->reticle = reticle; }
	void SetMatViewProjection(const XMMATRIX& matView, const XMMATRIX& matProjection) {
		this->matView = matView;
		this->matProjection = matProjection;
	}

private:
	//3Dレティクル
	Reticle3D* reticle = nullptr;
	//ビュー行列
	XMMATRIX matView = {};
	//プロジェクション行列
	XMMATRIX matProjection = {};
};
