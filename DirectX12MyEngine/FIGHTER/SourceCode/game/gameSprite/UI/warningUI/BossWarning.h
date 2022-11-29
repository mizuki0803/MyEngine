#pragma once
#include "Sprite.h"
#include "BossWarningSprites.h"
#include <memory>
#include <array>

/// <summary>
/// ボス登場警告
/// </summary>
class BossWarning
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="warningTime">警告時間</param>
	/// <returns>ボス登場警告</returns>
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

private: //静的メンバ変数
	//警告画面の画面赤の色の濃さ
	static const float warningColorAlpha;

private: //メンバ変数
	//画面赤用スプライト
	std::unique_ptr<Sprite> screenRedSprite;
	//警告用スプライト群
	std::array<std::unique_ptr<BossWarningSprites>, 2> bossWarningSprites;
	//画面を覆う色の濃さ
	float colorDeepNum = 0;
	//警告時間
	int32_t warningTime = 0;
	//警告時間タイマー
	int32_t warningTimer = 0;
	//削除するか
	bool isDelete = false;
};
