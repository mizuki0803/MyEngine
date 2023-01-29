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
	/// <param name="texture">テクスチャ</param>
	/// <param name="showPos">表示座標</param>
	/// <param name="isMoveRight">右向きに移動するか</param>
	/// <returns>警告ライン</returns>
	static WarningLine* Create(const Texture& texture, const Vector2& showPos, const bool isMoveRight);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="showPos">表示座標</param>
	/// <param name="isMoveRight">右向きに移動するか</param>
	/// <returns>成否</returns>
	bool Initialize(const Texture& texture, const Vector2& showPos, const bool isMoveRight);

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

	/// <summary>
	/// 警告中ライン移動
	/// </summary>
	void LineMove();

private: //メンバ変数
	//生成座標
	Vector2 createPos;
	//表示座標
	Vector2 showPos;
	//表示大きさ
	Vector2 showSize;
	//右に移動するか
	bool isMoveRight = true;
};
