#pragma once
#include "NumberSprite.h"
#include <memory>
#include <array>

/// <summary>
/// ステージ選択画面UI
/// </summary>
class StageSelectUI
{
public:
	//行動フェーズ
	enum class ActionPhase {
		Stay,		//停止
		InScreen,	//上に上がる
		Show,		//表示
		OutScreen,	//下に下がる
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ステージ選択画面UI</returns>
	static StageSelectUI* Create();

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
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
	/// テキストを画面内に動かす開始
	/// </summary>
	/// <param name="stageNum">表示するステージ番号</param>
	void TextInScreenStart(const int stageNum);

	/// <summary>
	/// テキストを画面外へ動かす開始
	/// </summary>
	void TextOutScreenStart();

	//getter
	bool GetIsShow() { return isShow; }

private: //メンバ関数
	/// <summary>
	/// テキストを待機させる
	/// </summary>
	void TextStay();

	/// <summary>
	/// テキストを画面内に動かす開始
	/// </summary>
	void TextInScreen();

	/// <summary>
	/// テキストを画面外へ動かす
	/// </summary>
	void TextOutScreen();

private: //静的メンバ変数
	//行動遷移
	static void (StageSelectUI::* actionFuncTable[])();
	//画面内Y座標
	static const float inScreenPosY;
	//画面外Y座標
	static const float outScreenPosY;
	//数字の桁数
	static const int highScoreDigit = 3;
	//枠の中心と数字のY座標の距離
	static const float frameNumberDistanceY;
	//枠の中心とランクメダルのY座標の距離
	static const float frameRankMedalDistanceY;

private: //メンバ変数
	//行動
	ActionPhase phase = ActionPhase::Stay;
	//惑星名スプライト
	std::unique_ptr<Sprite> planetNameSprite;
	//ハイスコア枠スプライト
	std::unique_ptr<Sprite> highScoreFrameSprite;
	//数字スプライト配列
	std::array<std::unique_ptr<NumberSprite>, highScoreDigit> numberSprites;
	//ランクメダルスプライト
	std::unique_ptr<Sprite> rankMedalSprite;
	//操作説明スプライト
	std::unique_ptr<Sprite> howToPlaySprite;
	//描画するステージUI
	int stageNum = 0;
	//ハイスコアを表示するか
	bool isHighScoreShow = false;
	//移動時間タイマー
	int32_t moveTimer = 0;
	//画面外に移動開始する座標
	Vector2 moveOutScreenStartPos;
	//表示しているか
	bool isShow = false;
};