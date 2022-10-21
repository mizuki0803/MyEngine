#pragma once

/// <summary>
/// 敵を倒した数カウンター
/// </summary>
class EnemyDefeatCounter
{
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
	/// ハイスコア更新か確認
	/// </summary>
	static void CheckHighScore();

	//getter
	static int GetDefeatCount() { return defeatCounter; }
	static int GetHighScore() { return highScore; }

private: //静的メンバ変数
	//カウンター
	static int defeatCounter;
	//ハイスコア
	static int highScore;
};