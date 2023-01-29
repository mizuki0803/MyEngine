#pragma once
#include "Reticle3D.h"
#include "Reticle2D.h"

/// <summary>
/// 2Dレティクル簡易作成クラス
/// </summary>
class Reticle
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="distance">発射対象からの距離</param>
	/// <param name="size">大きさ</param>
	/// <returns>レティクル</returns>
	static Reticle* Create(const Texture& texture, float distance, const Vector2& size);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="distance">発射対象からの距離</param>
	/// <param name="size">大きさ</param>
	/// <returns>成否</returns>
	bool Initialize(const Texture& texture, float distance, const Vector2& size);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matWorld">ワールド行列</param>
	/// <param name="matView">ビュー行列</param>
	/// <param name="matProjection">プロジェクション行列</param>
	void Update(const XMMATRIX& matWorld, const XMMATRIX& matView, const XMMATRIX& matProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	Reticle3D* GetReticle3D() { return reticle3d.get(); }
	Reticle2D* GetReticle2D() { return reticle2d.get(); }

private:
	//3Dレティクル
	std::unique_ptr<Reticle3D> reticle3d;
	//2Dレティクル
	std::unique_ptr<Reticle2D> reticle2d;
};