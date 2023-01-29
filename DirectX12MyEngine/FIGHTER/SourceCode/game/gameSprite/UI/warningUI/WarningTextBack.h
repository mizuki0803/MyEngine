#pragma once
#include "Sprite.h"

/// <summary>
/// 警告テキスト背景
/// </summary>
class WarningTextBack : public Sprite
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="position">座標</param>
	/// <param name="size">大きさ</param>
	/// <returns>警告テキスト背景</returns>
	static WarningTextBack* Create(const Texture& texture, const Vector2& position, const Vector2 size);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="position">座標</param>
	/// <param name="size">大きさ</param>
	/// <returns>成否</returns>
	bool Initialize(const Texture& texture, const Vector2& position, const Vector2 size);

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

	//getter
	const Vector2& GetShowSize() { return showSize; }

private: //メンバ変数
	//表示大きさ
	Vector2 showSize;
};
