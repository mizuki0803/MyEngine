#pragma once
#include "Sprite.h"
#include "Reticle3D.h"
#include "Enemy.h"

/// <summary>
/// 2Dレティクル
/// </summary>
class Reticle2D : public Sprite
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="size">大きさ</param>
	/// <returns>レティクル2D</returns>
	static Reticle2D* Create(const Texture& texture, const Vector2& size);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="isFlipX">左右判定するか</param>
	/// <param name="isFlipY">上下反転するか</param>
	/// <returns>成否</returns>
	bool Initialize(const Texture& texture, const Vector2& anchorpoint, bool isFlipX, bool isFlipY) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//setter
	void SetWorldPos3d(const Vector3& worldPos3d) { this->worldPos3d = worldPos3d; }
	void SetMatViewProjection(const XMMATRIX& matView, const XMMATRIX& matProjection) {
		this->matView = matView;
		this->matProjection = matProjection;
	}

protected: //メンバ変数
	/// <summary>
	/// 通常移動
	/// </summary>
	virtual void Move();

protected: //静的メンバ変数
	//通常時の色
	static const XMFLOAT4 normalColor;

protected:
	//3Dレティクルのワールド座標
	Vector3 worldPos3d = {};
	//ビュー行列
	XMMATRIX matView = {};
	//プロジェクション行列
	XMMATRIX matProjection = {};
};
