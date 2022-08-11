#pragma once
#include "Sprite.h"

/// <summary>
/// ボス用HPバーフレーム
/// </summary>
class BossHPFrame : public Sprite
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス用HPバーフレーム</returns>
	static BossHPFrame* Create(UINT texNumber, const Vector2 position);
};
