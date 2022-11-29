#pragma once
#include "Sprite.h"

/// <summary>
/// 警告ライン
/// </summary>
class WarningLine : public Sprite
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>警告ライン</returns>
	static WarningLine* Create(const Vector2& createPos, const Vector2& stayPos);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

private: //メンバ関数
	/// <summary>
	/// 警告開始処理
	/// </summary>
	/// <param name="warningTimer">警告時間タイマー</param>
	void WarningStart(int32_t warningTimer);

	/// <summary>
	/// 警告中
	/// </summary>
	void Warning();

	/// <summary>
	/// 警告終了処理
	/// </summary>
	/// <param name="warningTimer">警告時間タイマー</param>
	void WarningEnd(int32_t warningTimer);

private: //メンバ変数
	//警告時間
	int32_t warningTime = 0;
	//警告時間タイマー
	int32_t warningTimer = 0;
};
