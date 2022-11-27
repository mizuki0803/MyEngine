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
	//描画するUI
	enum class DrawUI
	{
		Shot,	//ショット
		Charge,	//チャージ
		Boost,	//ブースト
		Brake,	//ブレーキ
		Rolling,//ローリング
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
	/// UI生成
	/// </summary>
	/// <param name="drawUI">作成するUI</param>
	/// <param name="nextDrawUI">次に作成するUI</param>
	void CreateUI(DrawUI drawUI, DrawUI nextDrawUI);

	//getter
	DrawUI GetNextDrawUI() { return nextDrawPhase; }

private: //メンバ変数
	//遊び方スプライト
	std::list<std::unique_ptr<HowToPlaySprite>> howToPlaySprites;
	//次に描画するUI
	DrawUI nextDrawPhase = DrawUI::Shot;
};