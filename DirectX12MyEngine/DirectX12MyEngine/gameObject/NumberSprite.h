#pragma once
#include "Sprite.h"

/// <summary>
/// 数字スプライト
/// </summary>
class NumberSprite : public Sprite
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>数字スプライト</returns>
	static NumberSprite* Create(UINT texNumber, const Vector2& position, const Vector2& size, const Vector2& texSize);

public: //メンバ関数
	/// <summary>
	/// 表示する番号をセット
	/// </summary>
	/// <param name="number">表示する番号(0～9, 10は.)</param>
	void SetNumber(const int number);
};