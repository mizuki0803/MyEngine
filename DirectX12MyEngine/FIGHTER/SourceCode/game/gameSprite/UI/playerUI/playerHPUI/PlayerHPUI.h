#pragma once
#include "PlayerHPBar.h"
#include "PlayerHPFrame.h"
#include <memory>

/// <summary>
/// プレイヤー用HPUI
/// </summary>
class PlayerHPUI
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>プレイヤー用HPUI</returns>
	static PlayerHPUI* Create(const Vector2& position, const int maxHP);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize(const Vector2& position, const int maxHP);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 被弾したのでバーの長さ変更情報をセット
	/// </summary>
	/// <param name="HP">プレイヤーのHP</param>
	void Damage(const int HP);

	/// <summary>
	/// 回復アイテム獲得状態を開始
	/// </summary>
	void ItemGet(const int HP);

private: //メンバ変数
	//HPバー
	std::unique_ptr<PlayerHPBar> hpBar;
	//HPフレーム
	std::unique_ptr<PlayerHPFrame> hpFrame;
};
