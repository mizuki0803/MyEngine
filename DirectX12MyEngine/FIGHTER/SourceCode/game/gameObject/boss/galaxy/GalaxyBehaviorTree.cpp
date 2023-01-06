#include "GalaxyBehaviorTree.h"
#include "Galaxy.h"

GalaxyBehaviorTree* GalaxyBehaviorTree::Create(Galaxy* galaxy)
{
	//ビヘイビアツリーのインスタンスを生成
	GalaxyBehaviorTree* galaxyBehaviorTree = new GalaxyBehaviorTree();
	if (galaxyBehaviorTree == nullptr) {
		assert(0);
		return nullptr;
	}

	// 初期化
	if (!galaxyBehaviorTree->Initialize(galaxy)) {
		delete galaxyBehaviorTree;
		assert(0);
		return nullptr;
	}

	return galaxyBehaviorTree;
}

bool GalaxyBehaviorTree::Initialize(Galaxy* galaxy)
{
	//ギャラクシーをセット
	this->galaxy = galaxy;

	//Selector,Sequencerの生成
	topSelector.reset(Selector::Create());
	appearModeSequencer.reset(Sequencer::Create());

	//木構造を作成
	MakeTree();

	return true;
}

void GalaxyBehaviorTree::Root()
{
	//関数自体をルートノードに見立て、ルートノード直下のノードを実行する
	topSelector->Select();
}

void GalaxyBehaviorTree::MakeTree()
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
		std::bind(&Galaxy::WaitMode, galaxy);
	topSelector->AddNode(waitMode);


	//死亡状態セレクター
	std::function<bool()> deadExplosion =
		std::bind(&Galaxy::DeadExplosion, galaxy);
	topSelector->AddNode(deadExplosion);
}

void GalaxyBehaviorTree::AppearModeSequenceNode()
{
	//登場状態シーケンスノード

	//時間経過
	std::function<bool()> appearModeCount =
		std::bind(&Galaxy::AppearModeCount, galaxy);
	appearModeSequencer->AddNode(appearModeCount);

	//登場降下
	std::function<bool()> appearFall =
		std::bind(&Galaxy::AppearFall, galaxy);
	appearModeSequencer->AddNode(appearFall);
}
