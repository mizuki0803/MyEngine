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
	std::function<bool()> fallMode =
		std::bind(&Boss::FallMode, boss);
	topSelector->AddNode(fallMode);

	std::function<bool()> attackModeSequence =
		std::bind(&Sequencer::Sequence, attackModeSequencer.get());
	topSelector->AddNode(attackModeSequence);

	std::function<bool()> waitModeSequence =
		std::bind(&Sequencer::Sequence, waitModeSequencer.get());
	topSelector->AddNode(waitModeSequence);

	std::function<bool()> deadMode =
		std::bind(&Boss::DeadMode, boss);
	topSelector->AddNode(deadMode);


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

	std::function<bool()> attackTypeTrackingSelect =
		std::bind(&Boss::AttackTypeTrackingSelect, boss);
	attackTypeSelector->AddNode(attackTypeTrackingSelect);

	std::function<bool()> attackTypeSuperGiantBulletSelect =
		std::bind(&Boss::AttackTypeSuperGiantBulletSelect, boss);
	attackTypeSelector->AddNode(attackTypeSuperGiantBulletSelect);

	std::function<bool()> attackTypeRotateSelect =
		std::bind(&Boss::AttackTypeRotateSelect, boss);
	attackTypeSelector->AddNode(attackTypeRotateSelect);

	std::function<bool()> attackTypeAvatarBodyBlowSelect =
		std::bind(&Boss::AttackTypeAvatarBodyBlowSelect, boss);
	attackTypeSelector->AddNode(attackTypeAvatarBodyBlowSelect);

	std::function<bool()> attackTypeAvatarGatlingSelect =
		std::bind(&Boss::AttackTypeAvatarGatlingSelect, boss);
	attackTypeSelector->AddNode(attackTypeAvatarGatlingSelect);

	std::function<bool()> attackTypeAvatarGiantBulletSelect =
		std::bind(&Boss::AttackTypeAvatarGiantBulletSelect, boss);
	attackTypeSelector->AddNode(attackTypeAvatarGiantBulletSelect);


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
	std::function<bool()> attackTypeTracking =
		std::bind(&Boss::AttackTypeTracking, boss);
	attackSelector->AddNode(attackTypeTracking);

	std::function<bool()> attackTypeSuperGiantBullet =
		std::bind(&Boss::AttackTypeSuperGiantBullet, boss);
	attackSelector->AddNode(attackTypeSuperGiantBullet);

	std::function<bool()> attackTypeRotate =
		std::bind(&Boss::AttackTypeRotate, boss);
	attackSelector->AddNode(attackTypeRotate);

	std::function<bool()> attackTypeAvatarBodyBlow =
		std::bind(&Boss::AttackTypeAvatarBodyBlow, boss);
	attackSelector->AddNode(attackTypeAvatarBodyBlow);

	std::function<bool()> attackTypeAvatarGatling =
		std::bind(&Boss::AttackTypeAvatarGatling, boss);
	attackSelector->AddNode(attackTypeAvatarGatling);

	std::function<bool()> attackTypeAvatarGiantBullet =
		std::bind(&Boss::AttackTypeAvatarGiantBullet, boss);
	attackSelector->AddNode(attackTypeAvatarGiantBullet);


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
