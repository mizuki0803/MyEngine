#include "Boss2BehaviorTree.h"
#include "Boss2.h"

Boss2BehaviorTree* Boss2BehaviorTree::Create(Boss2* boss2)
{
	//ビヘイビアツリーのインスタンスを生成
	Boss2BehaviorTree* boss2BehaviorTree = new Boss2BehaviorTree();
	if (boss2BehaviorTree == nullptr) {
		assert(0);
		return nullptr;
	}

	// 初期化
	if (!boss2BehaviorTree->Initialize(boss2)) {
		delete boss2BehaviorTree;
		assert(0);
		return nullptr;
	}

	return boss2BehaviorTree;
}

bool Boss2BehaviorTree::Initialize(Boss2* boss2)
{
	//ボス02をセット
	this->boss2 = boss2;

	//Selector,Sequencerの生成
	topSelector.reset(Selector::Create());
	appearModeSequencer.reset(Sequencer::Create());

	//木構造を作成
	MakeTree();

	return true;
}

void Boss2BehaviorTree::Root()
{
	//関数自体をルートノードに見立て、ルートノード直下のノードを実行する
	topSelector->Select();
}

void Boss2BehaviorTree::MakeTree()
{
	//ルートノード

	//登場状態
	std::function<bool()> appearModeSequence =
		std::bind(&Sequencer::Sequence, appearModeSequencer.get());
	topSelector->AddNode(appearModeSequence);
	//登場状態シーケンスノード
	AppearModeSequenceNode();

	//待機状態シーケンサー
	std::function<bool()> waitMode =
		std::bind(&Boss2::WaitMode, boss2);
	topSelector->AddNode(waitMode);


	//死亡状態セレクター
	std::function<bool()> deadExplosion =
		std::bind(&Boss2::DeadExplosion, boss2);
	topSelector->AddNode(deadExplosion);
}

void Boss2BehaviorTree::AppearModeSequenceNode()
{
	//登場状態シーケンスノード

	//時間経過
	std::function<bool()> appearModeCount =
		std::bind(&Boss2::AppearModeCount, boss2);
	appearModeSequencer->AddNode(appearModeCount);

	//登場降下
	std::function<bool()> appearFall =
		std::bind(&Boss2::AppearFall, boss2);
	appearModeSequencer->AddNode(appearFall);
}
