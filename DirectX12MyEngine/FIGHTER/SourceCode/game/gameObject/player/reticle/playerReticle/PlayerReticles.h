#pragma once
#include "PlayerNearReticle.h"
#include "PlayerFarReticle.h"

/// <summary>
/// 自機付属の2つのレティクル制御クラス
/// </summary>
class PlayerReticles
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns> 自機付属の2つのレティクル</returns>
	static PlayerReticles* Create();

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

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

	/// <summary>
	/// チャージ状態を開始する
	/// </summary>
	void ChargeModeStart();

	/// <summary>
	/// チャージ状態を終了する
	/// </summary>
	void ChargeModeEnd();

	/// <summary>
	/// 敵ロックオン
	/// </summary>
	/// <param name="enemy">ロックオン対象の敵</param>
	void LockonEnemy(Enemy* enemy);

	/// <summary>
	/// 敵ロックオン解除
	/// </summary>
	void UnlockonEnemy();

	//getter
	const Vector3 GetNearReticleWorldPos() { return nearReticle->GetReticle3D()->GetWorldPos(); }
	const Vector2& GetFarReticleScreenPosition() { return farReticle->GetReticle2D()->GetPosition(); }
	const float GetFarReticleSize() { return farReticle->GetReticle2D()->GetSize().x; }
	const bool GetIsChargeMode() { return farReticle->GetReticle2D()->GetIsChargeMode(); }
	const bool GetIsLockon() { return farReticle->GetReticle2D()->GetIsLockon(); }
	Enemy* GetLockonEnemy() { return farReticle->GetReticle2D()->GetLockonEnemy(); }

private:
	//レティクル(近)
	std::unique_ptr<PlayerNearReticle> nearReticle;
	//レティクル(遠)
	std::unique_ptr<PlayerFarReticle> farReticle;
};