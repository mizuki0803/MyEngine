#pragma once
#include "Reticle3D.h"
#include "PlayerFarReticle2D.h"

/// <summary>
/// 自機付属の2Dレティクル(遠)簡易作成クラス
/// </summary>
class PlayerFarReticle
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>自機付属の2Dレティクル(遠)簡易作成クラス</returns>
	static PlayerFarReticle* Create(UINT texNumber, float distance, const Vector2& normalSize, const Vector2& chargeModeSize);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize(UINT texNumber, float distance, const Vector2& normalSize, const Vector2& chargeModeSize);

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
	PlayerFarReticle2D* GetReticle2D() { return reticle2d.get(); }

private:
	//3Dレティクル
	std::unique_ptr<Reticle3D> reticle3d;
	//自機付属の2Dレティクル(遠)
	std::unique_ptr<PlayerFarReticle2D> reticle2d;
};