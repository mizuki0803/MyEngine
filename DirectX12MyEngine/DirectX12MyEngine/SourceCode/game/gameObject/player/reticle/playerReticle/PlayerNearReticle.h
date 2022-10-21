#pragma once
#include "Reticle3D.h"
#include "PlayerNearReticle2D.h"

/// <summary>
/// 自機付属の2Dレティクル(近)簡易作成クラス
/// </summary>
class PlayerNearReticle
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>自機付属の2Dレティクル(近)簡易作成クラス</returns>
	static PlayerNearReticle* Create(UINT texNumber, float distance, const Vector2& size);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize(UINT texNumber, float distance, const Vector2& size);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const XMMATRIX& matWorld, const XMMATRIX& matView, const XMMATRIX& matProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	Reticle3D* GetReticle3D() { return reticle3d.get(); }
	PlayerNearReticle2D* GetReticle2D() { return reticle2d.get(); }

private:
	//3Dレティクル
	std::unique_ptr<Reticle3D> reticle3d;
	//自機付属の2Dレティクル(近)
	std::unique_ptr<PlayerNearReticle2D> reticle2d;
};