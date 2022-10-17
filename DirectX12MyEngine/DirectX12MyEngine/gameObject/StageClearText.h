#pragma once
#include "Sprite.h"
#include <array>
#include <memory>

/// <summary>
/// ステージクリア時に表示するテキスト
/// </summary>
class StageClearText
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ステージクリア時に表示する文字</returns>
	static StageClearText* Create();

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	///	更新
	/// </summary>
	void Update();

	/// <summary>
	///	描画
	/// </summary>
	void Draw();

private: //メンバ関数
	/// <summary>
	/// 表示する文字数更新
	/// </summary>
	void UpdateDisplayNum();


private: //静的メンバ変数
	static const int textNum = 4;

private: //メンバ変数
	//ステージ名スプライト
	std::unique_ptr<Sprite> stageNameSprite;
	//文字用スプライト配列
	std::unique_ptr<Sprite> textSprites[textNum];
	//表示する文字数
	int displayTextNum = 0;
	//表示用タイマー
	int32_t displayTimer = 0;
};
