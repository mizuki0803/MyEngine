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
	selector1or2or3or4.reset(Selector::Create());

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
	//ルートノード直下
	std::function<bool()> fall =
		std::bind(&Boss::Fall, boss);
	topSelector->AddNode(fall);

	std::function<bool()> onetwothreefour =
		std::bind(&Selector::Select, selector1or2or3or4.get());
	topSelector->AddNode(onetwothreefour);



	std::function<bool()> otamesi =
		std::bind(&Boss::Otamesi, boss);
	selector1or2or3or4->AddNode(otamesi);

	std::function<bool()> otamesi2 =
		std::bind(&Boss::Otamesi2, boss);
	selector1or2or3or4->AddNode(otamesi2);

	std::function<bool()> otamesi3 =
		std::bind(&Boss::Otamesi3, boss);
	selector1or2or3or4->AddNode(otamesi3);

	std::function<bool()> otamesi4 =
		std::bind(&Boss::Otamesi4, boss);
	selector1or2or3or4->AddNode(otamesi4);
}
