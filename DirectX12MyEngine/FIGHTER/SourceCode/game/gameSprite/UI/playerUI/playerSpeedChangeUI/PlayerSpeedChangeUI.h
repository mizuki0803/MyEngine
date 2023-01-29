#pragma once
#include "PlayerSpeedChangeBar.h"
#include <memory>

/// <summary>
/// プレイヤー速度変更UI
/// </summary>
class PlayerSpeedChangeUI
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>	
	/// <param name="position">座標</param>
	/// <param name="maxGaugeNum">最大ゲージ量</param>
	/// <returns>プレイヤー速度変更UI</returns>
	static PlayerSpeedChangeUI* Create(const Vector2& position, const float maxGaugeNum);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="maxGaugeNum">最大ゲージ量</param>
	/// <returns>成否</returns>
	bool Initialize(const Vector2& position, const float maxGaugeNum);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// バーの長さ変更
	/// </summary>
	/// <param name="gaugeNum">プレイヤーのHP</param>
	void ChangeLength(const float gaugeNum);

private: //メンバ変数
	//バー
	std::unique_ptr<PlayerSpeedChangeBar> speedChangeBar;
	//フレーム
	std::unique_ptr<Sprite> speedChangeFrame;
};
