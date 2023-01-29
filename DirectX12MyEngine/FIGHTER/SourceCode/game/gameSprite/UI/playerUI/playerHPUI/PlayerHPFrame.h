#pragma once
#include "Sprite.h"

/// <summary>
/// プレイヤー用HPバーフレーム
/// </summary>
class PlayerHPFrame : public Sprite
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="position">座標</param>
	/// <returns>プレイヤー用HPバーフレーム</returns>
	static PlayerHPFrame* Create(const Texture& texture, const Vector2& position);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 回復アイテム獲得状態を開始
	/// </summary>
	void ItemGetModeStart();

private: //メンバ関数
	/// <summary>
	/// 回復アイテム獲得状態
	/// </summary>
	void ItemGetMode();

private: //静的メンバ変数
	//通常色
	static const XMFLOAT4 normalColor;
	//アイテム獲得色(黄色)
	static const XMFLOAT4 itemGetColor;

private: //メンバ変数
	//回復アイテム獲得状態か
	bool isItemGetMode = false;
	//回復アイテム獲得状態タイマー
	int itemGetModeTimer = 0;
	//色が通常色か
	bool isNormalColor = true;
};
