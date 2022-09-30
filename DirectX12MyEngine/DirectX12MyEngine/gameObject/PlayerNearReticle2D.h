#pragma once
#include "Reticle2D.h"

/// <summary>
/// 自機付属の2Dレティクル(近)
/// </summary>
class PlayerNearReticle2D : public Reticle2D
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>自機付属の2Dレティクル(近)</returns>
	static PlayerNearReticle2D* Create(UINT texNumber, const Vector2& size);

public: //メンバ関数
	/// <summary>
	/// チャージ状態を開始する
	/// </summary>
	void ChargeModeStart();

	/// <summary>
	/// チャージ状態を終了する
	/// </summary>
	void ChargeModeEnd();

private: //静的メンバ変数
	//チャージ状態時の色
	static const XMFLOAT4 chargeColor;
};
