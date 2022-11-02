#pragma once
#include "HowToPlaySprite.h"
#include <memory>
#include <list>

/// <summary>
/// 遊び方UI
/// </summary>
class HowToPlayUI
{
public:
	//次に描画するUI
	enum class NextDrawUI {
		Shot,	//ショット
		Charge,	//チャージ
		None,	//なし
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>遊び方UI</returns>
	static HowToPlayUI* Create();

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 「ショット」のUI生成
	/// </summary>
	void ShotUICreate();

	/// <summary>
	/// 「チャージ」のUI生成
	/// </summary>
	void ChargeUICreate();

	//getter
	NextDrawUI GetNextDrawUI() { return nextDrawPhase; }

private: //メンバ変数
	//遊び方スプライト
	std::list<std::unique_ptr<HowToPlaySprite>> howToPlaySprites;
	//次に描画するUI
	NextDrawUI nextDrawPhase = NextDrawUI::Shot;
};