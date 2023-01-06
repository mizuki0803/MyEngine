#include "GalaxyBehaviorTree.h"
#include "Galaxy.h"

GalaxyBehaviorTree* GalaxyBehaviorTree::Create(Galaxy* galaxy)
{
	//�r�w�C�r�A�c���[�̃C���X�^���X�𐶐�
	GalaxyBehaviorTree* galaxyBehaviorTree = new GalaxyBehaviorTree();
	if (galaxyBehaviorTree == nullptr) {
		assert(0);
		return nullptr;
	}

	// ������
	if (!galaxyBehaviorTree->Initialize(galaxy)) {
		delete galaxyBehaviorTree;
		assert(0);
		return nullptr;
	}

	return galaxyBehaviorTree;
}

bool GalaxyBehaviorTree::Initialize(Galaxy* galaxy)
{
	//�M�����N�V�[���Z�b�g
	this->galaxy = galaxy;

	//Selector,Sequencer�̐���
	topSelector.reset(Selector::Create());
	appearModeSequencer.reset(Sequencer::Create());

	//�؍\�����쐬
	MakeTree();

	return true;
}

void GalaxyBehaviorTree::Root()
{
	//�֐����̂����[�g�m�[�h�Ɍ����āA���[�g�m�[�h�����̃m�[�h�����s����
	topSelector->Select();
}

void GalaxyBehaviorTree::MakeTree()
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
		std::bind(&Galaxy::WaitMode, galaxy);
	topSelector->AddNode(waitMode);


	//���S��ԃZ���N�^�[
	std::function<bool()> deadExplosion =
		std::bind(&Galaxy::DeadExplosion, galaxy);
	topSelector->AddNode(deadExplosion);
}

void GalaxyBehaviorTree::AppearModeSequenceNode()
{
	//�o���ԃV�[�P���X�m�[�h

	//���Ԍo��
	std::function<bool()> appearModeCount =
		std::bind(&Galaxy::AppearModeCount, galaxy);
	appearModeSequencer->AddNode(appearModeCount);

	//�o��~��
	std::function<bool()> appearFall =
		std::bind(&Galaxy::AppearFall, galaxy);
	appearModeSequencer->AddNode(appearFall);
}
