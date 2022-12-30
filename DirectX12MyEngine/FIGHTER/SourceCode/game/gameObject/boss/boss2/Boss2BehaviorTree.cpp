#include "Boss2BehaviorTree.h"
#include "Boss2.h"

Boss2BehaviorTree* Boss2BehaviorTree::Create(Boss2* boss2)
{
	//�r�w�C�r�A�c���[�̃C���X�^���X�𐶐�
	Boss2BehaviorTree* boss2BehaviorTree = new Boss2BehaviorTree();
	if (boss2BehaviorTree == nullptr) {
		assert(0);
		return nullptr;
	}

	// ������
	if (!boss2BehaviorTree->Initialize(boss2)) {
		delete boss2BehaviorTree;
		assert(0);
		return nullptr;
	}

	return boss2BehaviorTree;
}

bool Boss2BehaviorTree::Initialize(Boss2* boss2)
{
	//�{�X02���Z�b�g
	this->boss2 = boss2;

	//Selector,Sequencer�̐���
	topSelector.reset(Selector::Create());
	appearModeSequencer.reset(Sequencer::Create());

	//�؍\�����쐬
	MakeTree();

	return true;
}

void Boss2BehaviorTree::Root()
{
	//�֐����̂����[�g�m�[�h�Ɍ����āA���[�g�m�[�h�����̃m�[�h�����s����
	topSelector->Select();
}

void Boss2BehaviorTree::MakeTree()
{
	//���[�g�m�[�h

	//�o����
	std::function<bool()> appearModeSequence =
		std::bind(&Sequencer::Sequence, appearModeSequencer.get());
	topSelector->AddNode(appearModeSequence);
	//�o���ԃV�[�P���X�m�[�h
	AppearModeSequenceNode();

	//�ҋ@��ԃV�[�P���T�[
	std::function<bool()> waitMode =
		std::bind(&Boss2::WaitMode, boss2);
	topSelector->AddNode(waitMode);


	//���S��ԃZ���N�^�[
	std::function<bool()> deadExplosion =
		std::bind(&Boss2::DeadExplosion, boss2);
	topSelector->AddNode(deadExplosion);
}

void Boss2BehaviorTree::AppearModeSequenceNode()
{
	//�o���ԃV�[�P���X�m�[�h

	//���Ԍo��
	std::function<bool()> appearModeCount =
		std::bind(&Boss2::AppearModeCount, boss2);
	appearModeSequencer->AddNode(appearModeCount);

	//�o��~��
	std::function<bool()> appearFall =
		std::bind(&Boss2::AppearFall, boss2);
	appearModeSequencer->AddNode(appearFall);
}
