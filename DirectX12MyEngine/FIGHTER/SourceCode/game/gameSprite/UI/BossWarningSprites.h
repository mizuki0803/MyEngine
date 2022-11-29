#pragma once
#include "Sprite.h"
#include <memory>
#include <array>

/// <summary>
/// ボス登場警告演出
/// </summary>
class BossWarning
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="warningTime">警告時間</param>
	/// <returns>ボス登場警告演出</returns>
	static BossWarning* Create(int32_t warningTime);

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

	//getter
	bool GetIsDelete() { return isDelete; }

private: //メンバ関数
	/// <summary>
	/// 警告
	/// </summary>
	void Warning();

	/// <summary>
	/// 警告開始処理
	/// </summary>
	/// <param name="warningTimer">警告時間タイマー</param>
	void WarningStart(int32_t warningTimer);

	/// <summary>
	/// 警告終了処理
	/// </summary>
	/// <param name="warningTimer">警告時間タイマー</param>
	void WarningEnd(int32_t warningTimer);

	/// <summary>
	/// テキストの色を変更し続ける
	/// </summary>
	void TextColorChange();

private: //静的メンバ変数
	//警告画面の画面赤の色の濃さ
	static const float warningColorAlpha;
	//警告「WARNING」文字色(赤が最大)
	static const DirectX::XMFLOAT4 warningTextColorMax;
	//警告「WARNING」文字色(赤が最小)
	static const DirectX::XMFLOAT4 warningTextColorMin;

private: //メンバ変数
	//画面赤用スプライト
	std::unique_ptr<Sprite> screenRedSprite;
	//文字スプライト
	std::unique_ptr<Sprite> textSprite;
	//文字背景用スプライト
	std::unique_ptr<Sprite> textBackSprite;
	//警告ラインスプライト
	std::array<std::unique_ptr<Sprite>, 2> lineSprite;
	//警告ライン背景用スプライト
	std::array<std::unique_ptr<Sprite>, 2> lineBackSprite;


	//文字スプライト
	std::unique_ptr<Sprite> textSpriteDown;
	//文字背景用スプライト
	std::unique_ptr<Sprite> textBackSpriteDown;
	//警告ラインスプライト
	std::array<std::unique_ptr<Sprite>, 2> lineSpriteDown;
	//警告ライン背景用スプライト
	std::array<std::unique_ptr<Sprite>, 2> lineBackSpriteDown;

	//画面を覆う色の濃さ
	float colorDeepNum = 0;
	//テキスト色変更用タイマー
	int textColorChangeTimer = 0;
	//色を濃くするか
	bool isTextColorDeep = false;
	//警告時間
	int32_t warningTime = 0;
	//警告時間タイマー
	int32_t warningTimer = 0;
	//削除するか
	bool isDelete = false;
};
