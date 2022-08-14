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
	//行動遷移ツリーを作る
	selector.reset(Selector::Create());
	selector->AddNode(boss->Otamesi);
	selector->AddNode(boss->Otamesi2);


	return true;
}

void BossBehaviorTree::Root()
{
	if (selector->Select()) {
		DebugText::GetInstance()->Print("Success", 300, 330);
	}
	else {
		DebugText::GetInstance()->Print("Failed", 300, 330);
	}
}
