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
	attackModeSequencer.reset(Sequencer::Create());
	attackTypeSelector.reset(Selector::Create());
	attackSelector.reset(Selector::Create());

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

	//�o���ԃV�[�P���T�[
	std::function<bool()> appearModeSequence =
		std::bind(&Sequencer::Sequence, appearModeSequencer.get());
	topSelector->AddNode(appearModeSequence);
	//�o���ԃV�[�P���X�m�[�h
	AppearModeSequenceNode();

	//�U����ԃV�[�P���T�[
	std::function<bool()> attackModeSequence =
		std::bind(&Sequencer::Sequence, attackModeSequencer.get());
	topSelector->AddNode(attackModeSequence);
	//�o���ԃV�[�P���X�m�[�h
	AttackModeSequenceNode();

	//�ҋ@���
	std::function<bool()> waitMode =
		std::bind(&Galaxy::WaitMode, galaxy);
	topSelector->AddNode(waitMode);

	//�U������p�[�c�ύX���
	std::function<bool()> attackPartChangeMode =
		std::bind(&Galaxy::AttackPartChangeMode, galaxy);
	topSelector->AddNode(attackPartChangeMode);

	//���S��ԃZ���N�^�[
	std::function<bool()> deadExplosion =
		std::bind(&Galaxy::DeadExplosion, galaxy);
	topSelector->AddNode(deadExplosion);
}

void GalaxyBehaviorTree::AppearModeSequenceNode()
{
	//�o���ԃV�[�P���X�m�[�h

	//�o��
	std::function<bool()> appearMode =
		std::bind(&Galaxy::AppearMode, galaxy);
	appearModeSequencer->AddNode(appearMode);

	//UI����
	std::function<bool()> appearUICreate =
		std::bind(&Galaxy::AppearUICreate, galaxy);
	appearModeSequencer->AddNode(appearUICreate);
}

void GalaxyBehaviorTree::AttackModeSequenceNode()
{
	//�U����ԃV�[�P���X�m�[�h

	//�U����ԊǗ�
	std::function<bool()> attackMode =
		std::bind(&Galaxy::AttackMode, galaxy);
	attackModeSequencer->AddNode(attackMode);

	//���ˍ��W�X�V
	std::function<bool()> updateBulletShotPos =
		std::bind(&Galaxy::UpdateBulletShotPos, galaxy);
	attackModeSequencer->AddNode(updateBulletShotPos);

	//�U�����e�Z���N�^�[
	std::function<bool()> attackTypeSelect =
		std::bind(&Selector::Select, attackTypeSelector.get());
	attackModeSequencer->AddNode(attackTypeSelect);
	//�U�����e�Z���N�g�m�[�h
	AttackTypeSelectNode();

	//�U���Z���N�^�[
	std::function<bool()> attackSelect =
		std::bind(&Selector::Select, attackSelector.get());
	attackModeSequencer->AddNode(attackSelect);
	//�U����ԓ����E�U���Z���N�g�m�[�h
	AttackSelectNode();
}

void GalaxyBehaviorTree::AttackTypeSelectNode()
{
	//�U�����e�Z���N�g�m�[�h

	//�U�����e�ݒ���J�n����
	std::function<bool()> attackTypeSelectStart =
		std::bind(&Galaxy::AttackTypeSelectStart, galaxy);
	attackTypeSelector->AddNode(attackTypeSelectStart);


	//�U�����e:����(��C)��ݒ肷��
	std::function<bool()> attackTypeRapidFireCannonSelect =
		std::bind(&Galaxy::AttackTypeRapidFireCannonSelect, galaxy);
	attackTypeSelector->AddNode(attackTypeRapidFireCannonSelect);


	//�U�����e:�Ή�����(�D��)��ݒ肷��
	std::function<bool()> attackTypeFlamethrowerBowSelect =
		std::bind(&Galaxy::AttackTypeFlamethrowerBowSelect, galaxy);
	attackTypeSelector->AddNode(attackTypeFlamethrowerBowSelect);


	//�U�����e:����ݒ肷��
	std::function<bool()> attackTypeASelect =
		std::bind(&Galaxy::AttackTypeASelect, galaxy);
	attackTypeSelector->AddNode(attackTypeASelect);
}

void GalaxyBehaviorTree::AttackSelectNode()
{
	//�U���Z���N�g�m�[�h

	//�U�����e;����(��C)�����s����
	std::function<bool()> attackTypeRapidFireCannon =
		std::bind(&Galaxy::AttackTypeRapidFireCannon, galaxy);
	attackSelector->AddNode(attackTypeRapidFireCannon);


	//�U�����e:�Ή�����(�D��)�����s����
	std::function<bool()> attackTypeFlamethrowerBow =
		std::bind(&Galaxy::AttackTypeFlamethrowerBow, galaxy);
	attackSelector->AddNode(attackTypeFlamethrowerBow);


	//�U�����e;�������s����
	std::function<bool()> attackTypeA =
		std::bind(&Galaxy::AttackTypeA, galaxy);
	attackSelector->AddNode(attackTypeA);
}
