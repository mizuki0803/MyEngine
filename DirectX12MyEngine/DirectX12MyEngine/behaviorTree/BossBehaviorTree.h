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
	void MakeTree();

	/// <summary>
	/// 攻撃状態シーケンスノード
	/// </summary>
	void AttackModeSequenceNode();

	/// <summary>
	/// 攻撃内容セレクトノード
	/// </summary>
	void AttackTypeSelectNode();

	/// <summary>
	/// 攻撃状態動きセレクトノード
	/// </summary>
	void AttackModeActionSelectNode();

	/// <summary>
	/// 攻撃状態動き・回転対象セレクトノード
	/// </summary>
	void AttackModeRotaSelectNode();

	/// <summary>
	/// 攻撃状態動き・攻撃セレクトノード
	/// </summary>
	void AttackSelectNode();

	/// <summary>
	/// 待機状態シーケンスノード
	/// </summary>
	void WaitModeSequenceNode();

	/// <summary>
	/// 待機状態動きセレクトノード
	/// </summary>
	void WaitModeActionSelectNode();

	/// <summary>
	/// 待機状態動き・回転対象セレクトノード
	/// </summary>
	void WaitModeRotaSelectNode();

private:
	//ボス
	Boss* boss = nullptr;
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