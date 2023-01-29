#pragma once
#include "Sprite.h"

/// <summary>
/// 警告ライン背景
/// </summary>
class WarningLineBack : public Sprite
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="showPos">表示座標</param>
	/// <param name="showSize">表示サイズ</param>
	/// <returns>警告ライン背景</returns>
	static WarningLineBack* Create(const Texture& texture, const Vector2& showPos, const Vector2 showSize);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="showPos">表示座標</param>
	/// <param name="showSize">表示サイズ</param>
	/// <returns>成否</returns>
	bool Initialize(const Texture& texture, const Vector2& showPos, const Vector2 showSize);

	/// <summary>
	/// 開始時に必要な情報をセット
	/// </summary>
	/// <param name="createPos">生成座標</param>
	void StartSet(const Vector2& createPos);

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
	const Vector2& GetShowPos() { return showPos; }
	const Vector2& GetShowSize() { return showSize; }

private: //メンバ変数
	//生成座標
	Vector2 createPos;
	//表示座標
	Vector2 showPos;
	//表示大きさ
	Vector2 showSize;
};
