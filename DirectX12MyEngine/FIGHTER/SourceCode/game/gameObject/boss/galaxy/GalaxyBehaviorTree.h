#pragma once
#include "Selector.h"
#include "Sequencer.h"
#include <d3dx12.h>

class Galaxy;

/// <summary>
/// ステージ2ボスの行動遷移ビヘイビアツリー
/// </summary>
class GalaxyBehaviorTree
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ギャラクシーの行動遷移ビヘイビアツリー</returns>
	static GalaxyBehaviorTree* Create(Galaxy* galaxy);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Galaxy* galaxy);

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
	/// 攻撃セレクトノード
	/// </summary>
	void AttackSelectNode();

private:
	//ギャラクシー
	Galaxy* galaxy = nullptr;
	//セレクター
	std::unique_ptr<Selector> topSelector;
	//シーケンサー
	std::unique_ptr<Sequencer> appearModeSequencer;
	//シーケンサー
	std::unique_ptr<Sequencer> attackModeSequencer;
	//セレクター
	std::unique_ptr<Selector> attackTypeSelector;
	//セレクター
	std::unique_ptr<Selector> attackSelector;
};