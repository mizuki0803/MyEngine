#pragma once
#include "NumberSprite.h"
#include <vector>
#include <memory>

/// <summary>
/// ステージリザルトUI
/// </summary>
class StageResultUI
{
private: //リザルト行動のフェーズ
	enum class ResultActionPhase {
		UpdateDisplayNum,	//表示用数字更新
		RankMedalSet,		//スコアに応じた色のメダルをセット
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="enemyDefeatNum">敵撃破数</param>
	/// <param name="enemyDefeatRank">ランク</param>
	/// <returns>ステージクリア情報結果UI</returns>
	static StageResultUI* Create(const int enemyDefeatNum, const int enemyDefeatRank);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(const int enemyDefeatNum, const int enemyDefeatRank);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	bool GetIsResultEnd() { return isResultEnd; }
	bool GetIsDrawButtonSprite() { return isDrawButtonSprite; }

	//setter
	void SetIsDrawButtonSprite(bool isDraw) { isDrawButtonSprite = isDraw; }

private: //メンバ関数
	/// <summary>
	/// 表示用撃破数更新
	/// </summary>
	void UpdateDisplayNum();

	/// <summary>
	///	数字スプライト更新
	/// </summary>
	void UpdateNumberSprite();

	/// <summary>
	/// ランクに応じてメダルをセットする行動
	/// </summary>
	void RankMedalSetAction();

private: //静的メンバ変数
	//リザルトの行動遷移
	static void (StageResultUI::* resultActionPhaseFuncTable[])();

private: //メンバ変数
	//枠スプライト
	std::unique_ptr<Sprite> frameSprite;
	//数字スプライト
	std::vector<std::unique_ptr<NumberSprite>> numberSprites;
	//ランクメダルスプライト
	std::unique_ptr<Sprite> rankMedalSprite;
	//Bボタンスプライト
	std::unique_ptr<Sprite> bButtonSprite;
	//リザルト行動
	ResultActionPhase resultActionPhase = ResultActionPhase::UpdateDisplayNum;
	//行動用タイマー
	int32_t actionTimer = 0;
	//取得用撃破数
	int enemyDefeatNum = 0;
	//取得用撃破数の桁数
	int enemyDefeatNumDigit = 0;
	//表示用撃破数
	int enemyDefeatDisplayNum = 0;
	//表示用撃破数の桁数
	int enemyDefeatDisplayNumDigit = 0;
	//リザルトを表示し終えたか
	bool isResultEnd = false;
	//ボタンスプライトを描画するか
	bool isDrawButtonSprite = false;
};