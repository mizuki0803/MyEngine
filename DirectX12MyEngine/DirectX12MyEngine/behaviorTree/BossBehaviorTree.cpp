#include "BossBehaviorTree.h"
#include "Boss.h"
#include "DebugText.h"

BossBehaviorTree* BossBehaviorTree::Create(Boss* boss)
{
	//�r�w�C�r�A�c���[�̃C���X�^���X�𐶐�
	BossBehaviorTree* bossBehaviorTree = new BossBehaviorTree();
	if (bossBehaviorTree == nullptr) {
		assert(0);
		return nullptr;
	}

	// ������
	if (!bossBehaviorTree->Initialize(boss)) {
		delete bossBehaviorTree;
		assert(0);
		return nullptr;
	}

	return bossBehaviorTree;
}

bool BossBehaviorTree::Initialize(Boss* boss)
{	
	//Selector,Sequencer�̐���
	topSequencer.reset(Sequencer::Create());
	selector1or2.reset(Selector::Create());
	selector3or4.reset(Selector::Create());

	//�؍\�����쐬
	MakeTree(boss);


	return true;
}

void BossBehaviorTree::Root()
{
	//�֐����̂����[�g�m�[�h�Ɍ����āA���[�g�m�[�h������Sequencer�����s����
	if (topSequencer->Sequence()) {
		DebugText::GetInstance()->Print("Success", 300, 330);
	}
	else {
		DebugText::GetInstance()->Print("Failed", 300, 330);
	}
}

void BossBehaviorTree::MakeTree(Boss* boss)
{
	//���[�g�m�[�h������Sequencer
	std::function<bool()> onetwo =
		std::bind(&Selector::Select, selector1or2.get());
	topSequencer->AddNode(onetwo);
	std::function<bool()> threefour =
		std::bind(&Selector::Select, selector3or4.get());
	topSequencer->AddNode(threefour);


	std::function<bool()> otamesi =
		std::bind(&Boss::Otamesi, boss);
	selector1or2->AddNode(otamesi);
	std::function<bool()> otamesi2 =
		std::bind(&Boss::Otamesi2, boss);
	selector1or2->AddNode(otamesi2);


	std::function<bool()> otamesi3 =
		std::bind(&Boss::Otamesi3, boss);
	selector3or4->AddNode(otamesi3);
	std::function<bool()> otamesi4 =
		std::bind(&Boss::Otamesi4, boss);
	selector3or4->AddNode(otamesi4);
}
