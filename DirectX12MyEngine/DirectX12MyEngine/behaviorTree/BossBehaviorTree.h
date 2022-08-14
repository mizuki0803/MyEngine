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
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ボスの行動遷移ビヘイビアツリー</returns>
	static BossBehaviorTree* Create(Boss* boss);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize(Boss* boss);


	void Root();

private:
	//セレクター
	std::unique_ptr<Selector> selector;
	//シーケンサー
	std::unique_ptr<Sequencer> sequencer;
};