#pragma once
#include "Selector.h"
#include "Sequencer.h"
#include <d3dx12.h>

class Boss;

/// <summary>
/// ボスの行動遷移ビヘイビアツリー
/// </summary>
class BossBehaviorTree
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ボスの行動遷移ビヘイビアツリー</returns>
	static BossBehaviorTree* Create(Boss* boss);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize(Boss* boss);

	/// <summary>
	/// 行動遷移
	/// </summary>
	void Root();

private: //メンバ関数
	/// <summary>
	/// 木構造の作成
	/// </summary>
	void MakeTree(Boss* boss);

private:
	//セレクター
	std::unique_ptr<Selector> topSelector;
	//シーケンサー
	std::unique_ptr<Sequencer> attackModeSequencer;
	//シーケンサー
	std::unique_ptr<Sequencer> waitModeSequencer;
	//セレクター
	std::unique_ptr<Selector> attackTypeSelector;
	//セレクター
	std::unique_ptr<Selector> attackModeActionSelector;
	//セレクター
	std::unique_ptr<Selector> attackModeRotaSelector;
	//セレクター
	std::unique_ptr<Selector> attackSelector;
	//セレクター
	std::unique_ptr<Selector> waitModeActionSelector;
	//セレクター
	std::unique_ptr<Selector> waitModeRotaSelector;
};