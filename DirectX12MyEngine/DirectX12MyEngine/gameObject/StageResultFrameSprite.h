#pragma once
#include "Sprite.h"

/// <summary>
/// ステージリザルトの枠スプライト
/// </summary>
class StageResultFrameSprite : public Sprite
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ステージリザルトの枠スプライト</returns>
	static StageResultFrameSprite* Create(UINT texNumber, const Vector2& position, const Vector2& size, const Vector2& texSize);
};