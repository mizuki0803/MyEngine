#pragma once
#include <array>

/// <summary>
/// 敵を倒した数カウンター
/// </summary>
class EnemyDefeatCounter
{
public: //enum
	//スコアランク
	enum class EnemyDefeatRank {
		Gold,	//金
		Silver,	//銀
		Bronze,	//銅
		None,	//未設定
	};

public:
	//ランクのボーダーライン
	struct RankBorderline {
		int goldRankLine = 0;	//ランク金のボーダーライン
		int silverRankLine = 0;	//ランク銀のボーダーライン
	};


public: //静的メンバ関数
	/// <summary>
	/// カウントを増やす
	/// </summary>
	/// <param name="count">増やす数</param>
	static void AddCounter(int count = 1);

	/// <summary>
	/// カウンターリセット
	/// </summary>
	static void CounterReset();

	/// <summary>
	/// スコアを確定させる
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
	static void ScoreConfirm(int stageNum);

	//getter
	static int GetDefeatCount() { return defeatCounter; }
	static EnemyDefeatRank GetRank() { return scoreRank; }
	static int GetHighScore(int stageNum) { return highScore[stageNum]; }
	static EnemyDefeatRank GetBestRank(int stageNum) { return bestScoreRank[stageNum]; }

private: //メンバ関数
	/// <summary>
	/// ハイスコア更新か確認
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
	static void CheckHighScore(int stageNum);

	/// <summary>
	/// どのランクかチェック
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
	/// <param name="score">撃破スコア</param>
	static void CheckRank(int stageNum, int score);

private: //静的メンバ変数
	//カウンター
	static int defeatCounter;
	//ランク
	static EnemyDefeatRank scoreRank;
	//ハイスコア
	static std::array<int, 2> highScore;
	//ベストランク
	static std::array<EnemyDefeatRank, 2> bestScoreRank;
	//ランクのボーダーライン
	static std::array<RankBorderline, 2> rankBorderline;
};