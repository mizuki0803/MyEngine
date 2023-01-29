#pragma once
#include "Sprite.h"

/// <summary>
/// プレイヤー速度変更UIバー
/// </summary>
class PlayerSpeedChangeBar : public Sprite
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="position">座標</param>
	/// <param name="maxGaugeNum">最大ゲージ量</param>
	/// <returns>プレイヤー速度変更UIバー</returns>
	static PlayerSpeedChangeBar* Create(const Texture& texture, const Vector2& position, const float maxGaugeNum);

public: //メンバ関数
	/// <summary>
	/// バーの長さ変更
	/// </summary>
	/// <param name="HP">プレイヤーのHP</param>
	void ChangeLength(const float gaugeNum);


private: //メンバ変数
	//最大ゲージ数
	float maxGaugeNum = 0;
	//バーの長さ最大値
	const float lengthMax = 350;
};
