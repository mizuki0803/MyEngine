#pragma once
#include "Sprite.h"

/// <summary>
/// ボス用HPバー
/// </summary>
class BossHPBar : public Sprite
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス用HPバー</returns>
	static BossHPBar* Create(UINT texNumber, const Vector2 position, const int maxHP);

public: //メンバ関数

	/// <summary>
	/// バーの長さ変更
	/// </summary>
	/// <param name="HP">HP</param>
	void ChangeLength(const int HP);

private: //メンバ変数
	//最大HP
	int maxHP = 0;
	//バーの長さ最大値
	const float lengthMax = 400;
};
