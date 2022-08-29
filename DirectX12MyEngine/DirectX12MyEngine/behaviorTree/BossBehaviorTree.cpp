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
	topSelector.reset(Selector::Create());
	attackModeSequencer.reset(Sequencer::Create());
	waitModeSequencer.reset(Sequencer::Create());
	attackTypeSelector.reset(Selector::Create());
	attackModeActionSelector.reset(Selector::Create());
	attackModeRotaSelector.reset(Selector::Create());
	attackSelector.reset(Selector::Create());
	waitModeActionSelector.reset(Selector::Create());
	waitModeRotaSelector.reset(Selector::Create());

	//�؍\�����쐬
	MakeTree(boss);


	return true;
}

void BossBehaviorTree::Root()
{
	//�֐����̂����[�g�m�[�h�Ɍ����āA���[�g�m�[�h�����̃m�[�h�����s����
	if (topSelector->Select()) {
		DebugText::GetInstance()->Print("Success", 300, 330);
	}
	else {
		DebugText::GetInstance()->Print("Failed", 300, 330);
	}
}

void BossBehaviorTree::MakeTree(Boss* boss)
{
	//���[�g�m�[�h
	std::function<bool()> fall =
		std::bind(&Boss::Fall, boss);
	topSelector->AddNode(fall);

	std::function<bool()> attackModeSequence =
		std::bind(&Sequencer::Sequence, attackModeSequencer.get());
	topSelector->AddNode(attackModeSequence);

	std::function<bool()> waitModeSequence =
		std::bind(&Sequencer::Sequence, waitModeSequencer.get());
	topSelector->AddNode(waitModeSequence);


	//�U����ԃV�[�P���T�[
	std::function<bool()> attackMode =
		std::bind(&Boss::AttackModeCount, boss);
	attackModeSequencer->AddNode(attackMode);

	std::function<bool()> attackTypeSelect =
		std::bind(&Selector::Select, attackTypeSelector.get());
	attackModeSequencer->AddNode(attackTypeSelect);

	std::function<bool()> attackModeActionSelect =
		std::bind(&Selector::Select, attackModeActionSelector.get());
	attackModeSequencer->AddNode(attackModeActionSelect);


	//�ҋ@��ԃV�[�P���T�[
	std::function<bool()> waitMode =
		std::bind(&Boss::WaitModeCount, boss);
	waitModeSequencer->AddNode(waitMode);

	std::function<bool()> waitModeActionSelect =
		std::bind(&Selector::Select, waitModeActionSelector.get());
	waitModeSequencer->AddNode(waitModeActionSelect);


	//�U�����e�ݒ�Z���N�^�[
	std::function<bool()> attackTypeSelectStart =
		std::bind(&Boss::AttackTypeSelectStart, boss);
	attackTypeSelector->AddNode(attackTypeSelectStart);

	std::function<bool()> attackTypeASelect =
		std::bind(&Boss::AttackTypeASelect, boss);
	attackTypeSelector->AddNode(attackTypeASelect);

	std::function<bool()> attackTypeA2Select =
		std::bind(&Boss::AttackTypeA2Select, boss);
	attackTypeSelector->AddNode(attackTypeA2Select);

	std::function<bool()> attackTypeA3Select =
		std::bind(&Boss::AttackTypeA3Select, boss);
	attackTypeSelector->AddNode(attackTypeA3Select);

	std::function<bool()> attackTypeBSelect =
		std::bind(&Boss::AttackTypeBSelect, boss);
	attackTypeSelector->AddNode(attackTypeBSelect);

	std::function<bool()> attackTypeCSelect =
		std::bind(&Boss::AttackTypeCSelect, boss);
	attackTypeSelector->AddNode(attackTypeCSelect);

	std::function<bool()> attackTypeDSelect =
		std::bind(&Boss::AttackTypeDSelect, boss);
	attackTypeSelector->AddNode(attackTypeDSelect);


	//�U����ԓ����Z���N�^�[
	std::function<bool()> attackModeRotaSelect =
		std::bind(&Selector::Select, attackModeRotaSelector.get());
	attackModeActionSelector->AddNode(attackModeRotaSelect);

	std::function<bool()> attackSelect =
		std::bind(&Selector::Select, attackSelector.get());
	attackModeActionSelector->AddNode(attackSelect);


	//�U����Ԉڍs��]�Z���N�^�[
	std::function<bool()> attackModeMainBodyRota =
		std::bind(&Boss::AttackModeMainBodyRota, boss);
	attackModeRotaSelector->AddNode(attackModeMainBodyRota);

	std::function<bool()> attackModeAvatarRota =
		std::bind(&Boss::AttackModeAvatarRota, boss);
	attackModeRotaSelector->AddNode(attackModeAvatarRota);


	//�U���Z���N�^�[
	std::function<bool()> attackTypeA =
		std::bind(&Boss::AttackTypeA, boss);
	attackSelector->AddNode(attackTypeA);

	std::function<bool()> attackTypeA2 =
		std::bind(&Boss::AttackTypeA2, boss);
	attackSelector->AddNode(attackTypeA2);

	std::function<bool()> attackTypeA3 =
		std::bind(&Boss::AttackTypeA3, boss);
	attackSelector->AddNode(attackTypeA3);

	std::function<bool()> attackTypeB =
		std::bind(&Boss::AttackTypeB, boss);
	attackSelector->AddNode(attackTypeB);

	std::function<bool()> attackTypeC =
		std::bind(&Boss::AttackTypeC, boss);
	attackSelector->AddNode(attackTypeC);

	std::function<bool()> attackTypeD =
		std::bind(&Boss::AttackTypeD, boss);
	attackSelector->AddNode(attackTypeD);


	//�ҋ@��ԓ����Z���N�^�[
	std::function<bool()> waitModeRotaSelect =
		std::bind(&Selector::Select, waitModeRotaSelector.get());
	waitModeActionSelector->AddNode(waitModeRotaSelect);

	std::function<bool()> returnBasePosition =
		std::bind(&Boss::ReturnBasePosition, boss);
	waitModeActionSelector->AddNode(returnBasePosition);


	//�ҋ@��Ԉڍs��]�Z���N�^�[
	std::function<bool()> waitModeMainBodyRota =
		std::bind(&Boss::WaitModeMainBodyRota, boss);
	waitModeRotaSelector->AddNode(waitModeMainBodyRota);

	std::function<bool()> waitModeAvatarRota =
		std::bind(&Boss::WaitModeAvatarRota, boss);
	waitModeRotaSelector->AddNode(waitModeAvatarRota);
}
