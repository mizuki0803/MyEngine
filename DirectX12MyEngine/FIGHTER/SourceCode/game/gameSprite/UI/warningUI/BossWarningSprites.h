#pragma once
#include "Sprite.h"
#include "WarningLine.h"
#include "WarningLineBack.h"
#include "WarningText.h"
#include "WarningTextBack.h"
#include <memory>
#include <array>

/// <summary>
/// ボス登場警告演出1つ分のスプライト群
/// </summary>
class BossWarningSprites
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="leftTopPos">左上座標</param>
	/// <returns>ボス登場警告演出1つ分のスプライト群</returns>
	static BossWarningSprites* Create(const Vector2& leftTopPos);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="leftTopPos">左上座標</param>
	/// <returns>成否</returns>
	bool Initialize(const Vector2& leftTopPos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 警告
	/// </summary>
	void Warning();

	/// <summary>
	/// 警告開始処理
	/// </summary>
	/// <param name="warningTimer">警告時間タイマー</param>
	void WarningStart(float easeTimer);

	/// <summary>
	/// 警告終了処理
	/// </summary>
	/// <param name="warningTimer">警告時間タイマー</param>
	void WarningEnd(float easeTimer);

private: //メンバ変数
	//文字スプライト
	std::unique_ptr<WarningText> warningText;
	//文字背景用スプライト
	std::unique_ptr<WarningTextBack> warningTextBack;
	//警告ラインスプライト
	std::array<std::unique_ptr<WarningLine>, 2> warningLine;
	//警告ライン背景用スプライト
	std::array<std::unique_ptr<WarningLineBack>, 2> warningLineBack;
	//画面を覆う色の濃さ
	float colorDeepNum = 0;
	//テキスト色変更用タイマー
	int textColorChangeTimer = 0;
	//色を濃くするか
	bool isTextColorDeep = false;
};
