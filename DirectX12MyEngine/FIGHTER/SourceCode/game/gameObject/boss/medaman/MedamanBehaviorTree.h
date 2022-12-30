#pragma once
#include "Selector.h"
#include "Sequencer.h"
#include <d3dx12.h>

class Medaman;

/// <summary>
/// メダマーンの行動遷移ビヘイビアツリー
/// </summary>
class MedamanBehaviorTree
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>メダマーンの行動遷移ビヘイビアツリー</returns>
	static MedamanBehaviorTree* Create(Medaman* medaman);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize(Medaman* medaman);

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

	/// <summary>
	/// 死亡状態セレクトノード
	/// </summary>
	void DeadModeSelectNode();

private:
	//メダマーン
	Medaman* medaman = nullptr;
	//セレクター
	std::unique_ptr<Selector> topSelector;
	//シーケンサー
	std::unique_ptr<Sequencer> appearModeSequencer;
	//シーケンサー
	std::unique_ptr<Sequencer> attackModeSequencer;
	//シーケンサー
	std::unique_ptr<Sequencer> waitModeSequencer;
	//セレクター
	std::unique_ptr<Selector> deadModeSelector;
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