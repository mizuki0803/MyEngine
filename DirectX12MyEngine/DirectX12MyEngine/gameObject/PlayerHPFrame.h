#pragma once
#include "Sprite.h"

/// <summary>
/// プレイヤー用HPバーフレーム
/// </summary>
class PlayerHPFrame : public Sprite
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>プレイヤー用HPバーフレーム</returns>
	static PlayerHPFrame* Create(UINT texNumber, const Vector2 position);
};
