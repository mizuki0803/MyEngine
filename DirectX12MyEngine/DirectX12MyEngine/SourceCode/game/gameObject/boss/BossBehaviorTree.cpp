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
	//�{�X���Z�b�g
	this->boss = boss;

	//Selector,Sequencer�̐���
	topSelector.reset(Selector::Create());
	attackModeSequencer.reset(Sequencer::Create());
	waitModeSequencer.reset(Sequencer::Create());
	deadModeSelector.reset(Selector::Create());
	attackTypeSelector.reset(Selector::Create());
	attackModeActionSelector.reset(Selector::Create());
	attackModeRotaSelector.reset(Selector::Create());
	attackSelector.reset(Selector::Create());
	waitModeActionSelector.reset(Selector::Create());
	waitModeRotaSelector.reset(Selector::Create());

	//�؍\�����쐬
	MakeTree();

	return true;
}

void BossBehaviorTree::Root()
{
	//�֐����̂����[�g�m�[�h�Ɍ����āA���[�g�m�[�h�����̃m�[�h�����s����
	topSelector->Select();
}

void BossBehaviorTree::MakeTree()
{
	//���[�g�m�[�h

	//�~�����
	std::function<bool()> fallMode =
		std::bind(&Boss::FallMode, boss);
	topSelector->AddNode(fallMode);


	//�U����ԃV�[�P���T�[
	std::function<bool()> attackModeSequence =
		std::bind(&Sequencer::Sequence, attackModeSequencer.get());
	topSelector->AddNode(attackModeSequence);
	//�U����ԃV�[�P���X�m�[�h
	AttackModeSequenceNode();


	//�ҋ@��ԃV�[�P���T�[
	std::function<bool()> waitModeSequence =
		std::bind(&Sequencer::Sequence, waitModeSequencer.get());
	topSelector->AddNode(waitModeSequence);
	//�ҋ@��ԃV�[�P���X�m�[�h
	WaitModeSequenceNode();


	//���S��ԃZ���N�^�[
	std::function<bool()> deadModeSelect =
		std::bind(&Selector::Select, deadModeSelector.get());
	topSelector->AddNode(deadModeSelect);
	//���S��ԃZ���N�g�m�[�h
	DeadModeSelectNode();
}

void BossBehaviorTree::AttackModeSequenceNode()
{
	//�U����ԃV�[�P���X�m�[�h

	//���Ԍo��
	std::function<bool()> attackModeCount =
		std::bind(&Boss::AttackModeCount, boss);
	attackModeSequencer->AddNode(attackModeCount);


	//�U�����e�Z���N�^�[
	std::function<bool()> attackTypeSelect =
		std::bind(&Selector::Select, attackTypeSelector.get());
	attackModeSequencer->AddNode(attackTypeSelect);
	//�U�����e�Z���N�g�m�[�h
	AttackTypeSelectNode();


	//�U����ԓ����Z���N�^�[
	std::function<bool()> attackModeActionSelect =
		std::bind(&Selector::Select, attackModeActionSelector.get());
	attackModeSequencer->AddNode(attackModeActionSelect);
	//�U����ԓ����Z���N�g�m�[�h
	AttackModeActionSelectNode();
}

void BossBehaviorTree::AttackTypeSelectNode()
{
	//�U�����e�Z���N�g�m�[�h

	//�U�����e�ݒ���J�n����
	std::function<bool()> attackTypeSelectStart =
		std::bind(&Boss::AttackTypeSelectStart, boss);
	attackTypeSelector->AddNode(attackTypeSelectStart);


	//�U�����e:�Ǐ]��ݒ肷��
	std::function<bool()> attackTypeTrackingSelect =
		std::bind(&Boss::AttackTypeTrackingSelect, boss);
	attackTypeSelector->AddNode(attackTypeTrackingSelect);


	//�U�����e:������e��ݒ肷��
	std::function<bool()> attackTypeSuperGiantBulletSelect =
		std::bind(&Boss::AttackTypeSuperGiantBulletSelect, boss);
	attackTypeSelector->AddNode(attackTypeSuperGiantBulletSelect);


	//�U�����e:��]��ݒ肷��
	std::function<bool()> attackTypeRotateSelect =
		std::bind(&Boss::AttackTypeRotateSelect, boss);
	attackTypeSelector->AddNode(attackTypeRotateSelect);


	//�U�����e:���g�̓������ݒ肷��
	std::function<bool()> attackTypeAvatarBodyBlowSelect =
		std::bind(&Boss::AttackTypeAvatarBodyBlowSelect, boss);
	attackTypeSelector->AddNode(attackTypeAvatarBodyBlowSelect);


	//�U�����e:���g�K�g�����O�C��ݒ肷��
	std::function<bool()> attackTypeAvatarGatlingSelect =
		std::bind(&Boss::AttackTypeAvatarGatlingSelect, boss);
	attackTypeSelector->AddNode(attackTypeAvatarGatlingSelect);


	//�U�����e:���g����e��ݒ肷��
	std::function<bool()> attackTypeAvatarGiantBulletSelect =
		std::bind(&Boss::AttackTypeAvatarGiantBulletSelect, boss);
	attackTypeSelector->AddNode(attackTypeAvatarGiantBulletSelect);
}

void BossBehaviorTree::AttackModeActionSelectNode()
{
	//�U����ԓ����Z���N�g�m�[�h

	//�U����ԓ����E��]�ΏۃZ���N�^�[
	std::function<bool()> attackModeRotaSelect =
		std::bind(&Selector::Select, attackModeRotaSelector.get());
	attackModeActionSelector->AddNode(attackModeRotaSelect);
	//�U����ԓ����E��]�ΏۃZ���N�g�m�[�h
	AttackModeRotaSelectNode();


	//�U����ԓ����E�U���Z���N�^�[
	std::function<bool()> attackSelect =
		std::bind(&Selector::Select, attackSelector.get());
	attackModeActionSelector->AddNode(attackSelect);
	//�U����ԓ����E�U���Z���N�g�m�[�h
	AttackSelectNode();
}

void BossBehaviorTree::AttackModeRotaSelectNode()
{
	//�U����ԓ����E��]�ΏۃZ���N�g�m�[�h

	//�{�̂���]
	std::function<bool()> attackModeMainBodyRota =
		std::bind(&Boss::AttackModeMainBodyRota, boss);
	attackModeRotaSelector->AddNode(attackModeMainBodyRota);


	//���g����]
	std::function<bool()> attackModeAvatarRota =
		std::bind(&Boss::AttackModeAvatarRota, boss);
	attackModeRotaSelector->AddNode(attackModeAvatarRota);
}

void BossBehaviorTree::AttackSelectNode()
{
	//�U����ԓ����E�U���Z���N�g�m�[�h

	//�U�����e;�Ǐ]�����s����
	std::function<bool()> attackTypeTracking =
		std::bind(&Boss::AttackTypeTracking, boss);
	attackSelector->AddNode(attackTypeTracking);


	//�U�����e;������e�����s����
	std::function<bool()> attackTypeSuperGiantBullet =
		std::bind(&Boss::AttackTypeSuperGiantBullet, boss);
	attackSelector->AddNode(attackTypeSuperGiantBullet);


	//�U�����e;��]�����s����
	std::function<bool()> attackTypeRotate =
		std::bind(&Boss::AttackTypeRotate, boss);
	attackSelector->AddNode(attackTypeRotate);


	//�U�����e;���g�̓���������s����
	std::function<bool()> attackTypeAvatarBodyBlow =
		std::bind(&Boss::AttackTypeAvatarBodyBlow, boss);
	attackSelector->AddNode(attackTypeAvatarBodyBlow);


	//�U�����e;���g�K�g�����O�C�����s����
	std::function<bool()> attackTypeAvatarGatling =
		std::bind(&Boss::AttackTypeAvatarGatling, boss);
	attackSelector->AddNode(attackTypeAvatarGatling);


	//�U�����e;���g����e�����s����
	std::function<bool()> attackTypeAvatarGiantBullet =
		std::bind(&Boss::AttackTypeAvatarGiantBullet, boss);
	attackSelector->AddNode(attackTypeAvatarGiantBullet);
}

void BossBehaviorTree::WaitModeSequenceNode()
{
	//�ҋ@��ԃV�[�P���X�m�[�h

	//���Ԍo��
	std::function<bool()> waitModeCount =
		std::bind(&Boss::WaitModeCount, boss);
	waitModeSequencer->AddNode(waitModeCount);


	//�ҋ@��ԓ����Z���N�^�[
	std::function<bool()> waitModeActionSelect =
		std::bind(&Selector::Select, waitModeActionSelector.get());
	waitModeSequencer->AddNode(waitModeActionSelect);
	//�ҋ@��ԓ����Z���N�g�m�[�h
	WaitModeActionSelectNode();
}

void BossBehaviorTree::WaitModeActionSelectNode()
{
	//�ҋ@��ԓ����Z���N�g�m�[�h

	//�ҋ@��ԓ����E��]�ΏۃZ���N�^�[
	std::function<bool()> waitModeRotaSelect =
		std::bind(&Selector::Select, waitModeRotaSelector.get());
	waitModeActionSelector->AddNode(waitModeRotaSelect);
	//�ҋ@��ԓ����E��]�ΏۃZ���N�g�m�[�h
	WaitModeRotaSelectNode();


	//��ʒu�ɖ߂�
	std::function<bool()> returnBasePosition =
		std::bind(&Boss::ReturnBasePosition, boss);
	waitModeActionSelector->AddNode(returnBasePosition);
}

void BossBehaviorTree::WaitModeRotaSelectNode()
{
	//�ҋ@��ԓ����E��]�ΏۃZ���N�g�m�[�h

	//�{�̂���]
	std::function<bool()> waitModeMainBodyRota =
		std::bind(&Boss::WaitModeMainBodyRota, boss);
	waitModeRotaSelector->AddNode(waitModeMainBodyRota);


	//���g����]
	std::function<bool()> waitModeAvatarRota =
		std::bind(&Boss::WaitModeAvatarRota, boss);
	waitModeRotaSelector->AddNode(waitModeAvatarRota);
}

void BossBehaviorTree::DeadModeSelectNode()
{
	//���S��ԃZ���N�g�m�[�h

	//���S����
	std::function<bool()> deadExplosion =
		std::bind(&Boss::DeadExplosion, boss);
	deadModeSelector->AddNode(deadExplosion);


	//���S����
	std::function<bool()> deadFall =
		std::bind(&Boss::DeadFall, boss);
	deadModeSelector->AddNode(deadFall);
}
