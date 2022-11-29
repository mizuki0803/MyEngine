#pragma once
#include "Sprite.h"

/// <summary>
/// 警告テキスト「WARNING」
/// </summary>
class WarningText : public Sprite
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>警告テキスト「WARNING」</returns>
	static WarningText* Create(UINT texNumber, const Vector2& position);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(UINT texNumber, const Vector2& position);

	/// <summary>
	/// 警告開始処理
	/// </summary>
	/// <param name="easeTimer">イージングタイマー(0～1)/param>
	void WarningStart(float easeTimer);

	/// <summary>
	/// 警告終了処理
	/// </summary>
	/// <param name="easeTimer">イージングタイマー(0～1)</param>
	void WarningEnd(float easeTimer);

	/// <summary>
	/// テキストの色を変更
	/// </summary>
	void TextColorChange();

	//getter
	const Vector2& GetShowSize() { return showSize; }

private: //静的メンバ変数
	//警告「WARNING」文字色(赤が最大)
	static const DirectX::XMFLOAT4 warningTextColorMax;
	//警告「WARNING」文字色(赤が最小)
	static const DirectX::XMFLOAT4 warningTextColorMin;

private: //メンバ変数
	//表示大きさ
	Vector2 showSize;
	//テキスト色変更用タイマー
	int textColorChangeTimer = 0;
	//色を濃くするか
	bool isTextColorDeep = false;
};
