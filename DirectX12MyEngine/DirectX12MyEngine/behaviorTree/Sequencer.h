#pragma once
#include <d3dx12.h>
#include <vector>

/// <summary>
/// シーケンサー
/// </summary>
class Sequencer
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>シーケンサー</returns>
	static Sequencer* Create();

	/// <summary>
	/// シーケンス
	/// </summary>
	bool Sequence();

	/// <summary>
	/// 子ノードを追加する
	/// </summary>
	/// <param name="func()">関数</param>
	void AddNode(bool func());

private:
	typedef bool (*Func)();
	//子ノード
	std::vector<Func> nodes;
};