#pragma once
#include "Sprite.h"

/// <summary>
/// ボス用HPバーフレーム
/// </summary>
class BossHPFrame : public Sprite
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="position">座標</param>
	/// <returns>ボス用HPバーフレーム</returns>
	static BossHPFrame* Create(const Texture& texture, const Vector2& position);

public: //メンバ関数
	/// <summary>
	/// 開始時の移動
	/// </summary>
	/// <param name="moveTime">移動にかかる時間</param>
	/// <param name="timer">タイマー</param>
	void StartSetMove(int moveTime, int timer);

	/// <summary>
	/// 開始時の大きさ変更
	/// </summary>
	/// <param name="changeTime">大きさ変更にかかる時間</param>
	/// <param name="timer">タイマー</param>
	void StartSetChangeSize(int changeTime, int timer);


private: //メンバ変数
	//表示する座標
	Vector2 stayPosition;
	//枠の大きさ
	Vector2 frameSize;
};
