#include "BossBehaviorTree.h"
#include "Boss.h"
#include "DebugText.h"

BossBehaviorTree* BossBehaviorTree::Create(Boss* boss)
{
	//ビヘイビアツリーのインスタンスを生成
	BossBehaviorTree* bossBehaviorTree = new BossBehaviorTree();
	if (bossBehaviorTree == nullptr) {
		assert(0);
		return nullptr;
	}

	// 初期化
	if (!bossBehaviorTree->Initialize(boss)) {
		delete bossBehaviorTree;
		assert(0);
		return nullptr;
	}

	return bossBehaviorTree;
}

bool BossBehaviorTree::Initialize(Boss* boss)
{	
	//Selector,Sequencerの生成
	topSelector.reset(Selector::Create());
	attackModeSequencer.reset(Sequencer::Create());
	waitModeSequencer.reset(Sequencer::Create());

	//木構造を作成
	MakeTree(boss);


	return true;
}

void BossBehaviorTree::Root()
{
	//関数自体をルートノードに見立て、ルートノード直下のノードを実行する
	if (topSelector->Select()) {
		DebugText::GetInstance()->Print("Success", 300, 330);
	}
	else {
		DebugText::GetInstance()->Print("Failed", 300, 330);
	}
}

void BossBehaviorTree::MakeTree(Boss* boss)
{
	//ルートノード
	std::function<bool()> fall =
		std::bind(&Boss::Fall, boss);
	topSelector->AddNode(fall);

	std::function<bool()> attackSequencer =
		std::bind(&Sequencer::Sequence, attackModeSequencer.get());
	topSelector->AddNode(attackSequencer);

	std::function<bool()> waitSequencer =
		std::bind(&Sequencer::Sequence, waitModeSequencer.get());
	topSelector->AddNode(waitSequencer);


	//攻撃状態シーケンサー
	std::function<bool()> attackMode =
		std::bind(&Boss::AttackMode, boss);
	attackModeSequencer->AddNode(attackMode);

	std::function<bool()> changeAttackMode =
		std::bind(&Boss::ChangeRotAttackMode, boss);
	attackModeSequencer->AddNode(changeAttackMode);


	//待機状態シーケンサー
	std::function<bool()> waitMode =
		std::bind(&Boss::WaitMode, boss);
	waitModeSequencer->AddNode(waitMode);

	std::function<bool()> changeWaitMode =
		std::bind(&Boss::ChangeRotWaitMode, boss);
	waitModeSequencer->AddNode(changeWaitMode);
}
