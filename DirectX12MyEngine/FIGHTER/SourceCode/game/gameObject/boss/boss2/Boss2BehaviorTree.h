#pragma once
#include "Selector.h"
#include "Sequencer.h"
#include <d3dx12.h>

class Boss2;

/// <summary>
/// ステージ2ボスの行動遷移ビヘイビアツリー
/// </summary>
class Boss2BehaviorTree
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ボス02の行動遷移ビヘイビアツリー</returns>
	static Boss2BehaviorTree* Create(Boss2* boss2);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Boss2* boss2);

	/// <summary>
	/// 行動遷移
	/// </summary>
	void Root();

private: //メンバ関数
	/// <summary>
	/// 木構造の作成
	/// </summary>
	void MakeTree();

	/// <summary>
	/// 登場状態シーケンスノード
	/// </summary>
	void AppearModeSequenceNode();

private:
	//ボス02
	Boss2* boss2 = nullptr;
	//セレクター
	std::unique_ptr<Selector> topSelector;
	//シーケンサー
	std::unique_ptr<Sequencer> appearModeSequencer;
};