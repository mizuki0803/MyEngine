#pragma once
#include "NumberSprite.h"
#include <vector>
#include <memory>

/// <summary>
/// ステージリザルトUI
/// </summary>
class StageResultUI
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ステージクリア情報結果UI</returns>
	static StageResultUI* Create(const int enemyDefeatNum, bool isHighScore);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(const int enemyDefeatNum);

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

private: //メンバ変数
	//枠スプライト
	std::unique_ptr<Sprite> frameSprite;
	//数字スプライト
	std::vector<std::unique_ptr<NumberSprite>> numberSprites;
	//Bボタンスプライト
	std::unique_ptr<Sprite> bButtonSprite;
	//取得用撃破数
	int enemyDefeatNum = 0;
	//取得用撃破数の桁数
	int enemyDefeatNumDigit = 0;
	//表示用撃破数
	int enemyDefeatDisplayNum = 0;
	//表示用撃破数の桁数
	int enemyDefeatDisplayNumDigit = 0;
	//表示用撃破数更新用タイマー
	int32_t updateDisplayNumTimer = 0;
	//リザルトを表示し終えたか
	bool isResultEnd = false;
	//ボタンスプライトを描画するか
	bool isDrawButtonSprite = false;
};