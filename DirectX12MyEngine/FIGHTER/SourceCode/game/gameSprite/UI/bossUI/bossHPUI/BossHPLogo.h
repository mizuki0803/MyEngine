#pragma once
#include "Sprite.h"

/// <summary>
/// ボス用HPUIロゴ
/// </summary>
class BossHPLogo : public Sprite
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス用HPUIロゴ</returns>
	static BossHPLogo* Create(UINT texNumber, const Vector2& position);

public: //メンバ関数
	/// <summary>
	/// 開始時の移動
	/// </summary>
	/// <param name="moveTime">移動にかかる時間</param>
	/// <param name="timer">タイマー</param>
	void StartSetMove(int moveTime, int timer);


private: //メンバ変数
	//表示する座標
	Vector2 stayPosition;
};
