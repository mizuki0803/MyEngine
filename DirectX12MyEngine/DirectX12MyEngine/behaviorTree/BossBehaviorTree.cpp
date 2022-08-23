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
	attackTypeSelector.reset(Selector::Create());
	attackModeActionSelector.reset(Selector::Create());
	attackModeRotaSelector.reset(Selector::Create());
	attackSelector.reset(Selector::Create());
	waitModeActionSelector.reset(Selector::Create());
	waitModeRotaSelector.reset(Selector::Create());

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

	std::function<bool()> attackModeSequence =
		std::bind(&Sequencer::Sequence, attackModeSequencer.get());
	topSelector->AddNode(attackModeSequence);

	std::function<bool()> waitModeSequence =
		std::bind(&Sequencer::Sequence, waitModeSequencer.get());
	topSelector->AddNode(waitModeSequence);


	//攻撃状態シーケンサー
	std::function<bool()> attackMode =
		std::bind(&Boss::AttackModeCount, boss);
	attackModeSequencer->AddNode(attackMode);

	std::function<bool()> attackTypeSelect =
		std::bind(&Selector::Select, attackTypeSelector.get());
	attackModeSequencer->AddNode(attackTypeSelect);

	std::function<bool()> attackModeActionSelect =
		std::bind(&Selector::Select, attackModeActionSelector.get());
	attackModeSequencer->AddNode(attackModeActionSelect);


	//待機状態シーケンサー
	std::function<bool()> waitMode =
		std::bind(&Boss::WaitModeCount, boss);
	waitModeSequencer->AddNode(waitMode);

	std::function<bool()> waitModeActionSelect =
		std::bind(&Selector::Select, waitModeActionSelector.get());
	waitModeSequencer->AddNode(waitModeActionSelect);


	//攻撃内容設定セレクター
	std::function<bool()> attackTypeSelectStart =
		std::bind(&Boss::AttackTypeSelectStart, boss);
	attackTypeSelector->AddNode(attackTypeSelectStart);

	std::function<bool()> attackTypeASelect =
		std::bind(&Boss::AttackTypeASelect, boss);
	attackTypeSelector->AddNode(attackTypeASelect);

	std::function<bool()> attackTypeBSelect =
		std::bind(&Boss::AttackTypeBSelect, boss);
	attackTypeSelector->AddNode(attackTypeBSelect);


	//攻撃状態動きセレクター
	std::function<bool()> attackModeRotaSelect =
		std::bind(&Selector::Select, attackModeRotaSelector.get());
	attackModeActionSelector->AddNode(attackModeRotaSelect);

	std::function<bool()> attackSelect =
		std::bind(&Selector::Select, attackSelector.get());
	attackModeActionSelector->AddNode(attackSelect);


	//攻撃状態移行回転セレクター
	std::function<bool()> attackModeMainBodyRota =
		std::bind(&Boss::AttackModeMainBodyRota, boss);
	attackModeRotaSelector->AddNode(attackModeMainBodyRota);

	std::function<bool()> attackModeAvatarRota =
		std::bind(&Boss::AttackModeAvatarRota, boss);
	attackModeRotaSelector->AddNode(attackModeAvatarRota);


	//攻撃セレクター
	std::function<bool()> attackTypeA =
		std::bind(&Boss::AttackTypeA, boss);
	attackSelector->AddNode(attackTypeA);

	std::function<bool()> attackTypeB =
		std::bind(&Boss::AttackTypeB, boss);
	attackSelector->AddNode(attackTypeB);


	//待機状態動きセレクター
	std::function<bool()> waitModeRotaSelect =
		std::bind(&Selector::Select, waitModeRotaSelector.get());
	waitModeActionSelector->AddNode(waitModeRotaSelect);

	std::function<bool()> returnFixedPosition =
		std::bind(&Boss::ReturnFixedPosition, boss);
	waitModeActionSelector->AddNode(returnFixedPosition);


	//待機状態移行回転セレクター
	std::function<bool()> waitModeMainBodyRota =
		std::bind(&Boss::WaitModeMainBodyRota, boss);
	waitModeRotaSelector->AddNode(waitModeMainBodyRota);

	std::function<bool()> waitModeAvatarRota =
		std::bind(&Boss::WaitModeAvatarRota, boss);
	waitModeRotaSelector->AddNode(waitModeAvatarRota);
}
