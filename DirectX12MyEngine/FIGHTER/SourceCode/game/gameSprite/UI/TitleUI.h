#pragma once
#include "Sprite.h"
#include <memory>

/// <summary>
/// タイトルUI
/// </summary>
class TitleUI
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>タイトルUI</returns>
	static TitleUI* Create();

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 表示を終えるセット
	/// </summary>
	void SetShowEnd();

private: //メンバ関数
	/// <summary>
	/// ボタン表示をするしないを切り替え
	/// </summary>
	void DrawChangeButtonSprite();

	/// <summary>
	/// 表示を終える
	/// </summary>
	void ShowEnd();

private: //メンバ変数
	//タイトルロゴスプライト
	std::unique_ptr<Sprite> titleSprite;
	//Bボタンスプライト
	std::unique_ptr<Sprite> bButtonSprite;
	//ボタンスプライトを描画するか
	bool isDrawButtonSprite = true;
	//描画用タイマー
	int32_t drawTimer = 0;
	//表示を終えるか
	bool isShowEnd = false;
};