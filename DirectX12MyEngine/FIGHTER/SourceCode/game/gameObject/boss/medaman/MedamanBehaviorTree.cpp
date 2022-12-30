#include "MedamanBehaviorTree.h"
#include "Medaman.h"
#include "DebugText.h"

MedamanBehaviorTree* MedamanBehaviorTree::Create(Medaman* medaman)
{
	//�r�w�C�r�A�c���[�̃C���X�^���X�𐶐�
	MedamanBehaviorTree* medamanBehaviorTree = new MedamanBehaviorTree();
	if (medamanBehaviorTree == nullptr) {
		assert(0);
		return nullptr;
	}

	// ������
	if (!medamanBehaviorTree->Initialize(medaman)) {
		delete medamanBehaviorTree;
		assert(0);
		return nullptr;
	}

	return medamanBehaviorTree;
}

bool MedamanBehaviorTree::Initialize(Medaman* medaman)
{
	//���_�}�[�����Z�b�g
	this->medaman = medaman;

	//Selector,Sequencer�̐���
	topSelector.reset(Selector::Create());
	appearModeSequencer.reset(Sequencer::Create());
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

void MedamanBehaviorTree::Root()
{
	//�֐����̂����[�g�m�[�h�Ɍ����āA���[�g�m�[�h�����̃m�[�h�����s����
	topSelector->Select();
}

void MedamanBehaviorTree::MakeTree()
{
	//���[�g�m�[�h

	//�o����
	std::function<bool()> appearModeSequence =
		std::bind(&Sequencer::Sequence, appearModeSequencer.get());
	topSelector->AddNode(appearModeSequence);
	//�o���ԃV�[�P���X�m�[�h
	AppearModeSequenceNode();


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

void MedamanBehaviorTree::AppearModeSequenceNode()
{
	//�o���ԃV�[�P���X�m�[�h

	//���Ԍo��
	std::function<bool()> appearModeCount =
		std::bind(&Medaman::AppearModeCount, medaman);
	appearModeSequencer->AddNode(appearModeCount);

	//�o��~��
	std::function<bool()> appearFall =
		std::bind(&Medaman::AppearFall, medaman);
	appearModeSequencer->AddNode(appearFall);
}

void MedamanBehaviorTree::AttackModeSequenceNode()
{
	//�U����ԃV�[�P���X�m�[�h

	//���Ԍo��
	std::function<bool()> attackModeCount =
		std::bind(&Medaman::AttackModeCount, medaman);
	attackModeSequencer->AddNode(attackModeCount);

	//���ˍ��W�X�V
	std::function<bool()> updateBulletShotPos =
		std::bind(&Medaman::UpdateBulletShotPos, medaman);
	attackModeSequencer->AddNode(updateBulletShotPos);


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

void MedamanBehaviorTree::AttackTypeSelectNode()
{
	//�U�����e�Z���N�g�m�[�h

	//�U�����e�ݒ���J�n����
	std::function<bool()> attackTypeSelectStart =
		std::bind(&Medaman::AttackTypeSelectStart, medaman);
	attackTypeSelector->AddNode(attackTypeSelectStart);


	//�U�����e:�Ǐ]��ݒ肷��
	std::function<bool()> attackTypeTrackingSelect =
		std::bind(&Medaman::AttackTypeTrackingSelect, medaman);
	attackTypeSelector->AddNode(attackTypeTrackingSelect);


	//�U�����e:������e��ݒ肷��
	std::function<bool()> attackTypeSuperGiantBulletSelect =
		std::bind(&Medaman::AttackTypeSuperGiantBulletSelect, medaman);
	attackTypeSelector->AddNode(attackTypeSuperGiantBulletSelect);


	//�U�����e:��]��ݒ肷��
	std::function<bool()> attackTypeRotateSelect =
		std::bind(&Medaman::AttackTypeRotateSelect, medaman);
	attackTypeSelector->AddNode(attackTypeRotateSelect);


	//�U�����e:���g�̓������ݒ肷��
	std::function<bool()> attackTypeAvatarBodyBlowSelect =
		std::bind(&Medaman::AttackTypeAvatarBodyBlowSelect, medaman);
	attackTypeSelector->AddNode(attackTypeAvatarBodyBlowSelect);


	//�U�����e:���g�K�g�����O�C��ݒ肷��
	std::function<bool()> attackTypeAvatarGatlingSelect =
		std::bind(&Medaman::AttackTypeAvatarGatlingSelect, medaman);
	attackTypeSelector->AddNode(attackTypeAvatarGatlingSelect);


	//�U�����e:���g����e��ݒ肷��
	std::function<bool()> attackTypeAvatarGiantBulletSelect =
		std::bind(&Medaman::AttackTypeAvatarGiantBulletSelect, medaman);
	attackTypeSelector->AddNode(attackTypeAvatarGiantBulletSelect);
}

void MedamanBehaviorTree::AttackModeActionSelectNode()
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

void MedamanBehaviorTree::AttackModeRotaSelectNode()
{
	//�U����ԓ����E��]�ΏۃZ���N�g�m�[�h

	//�{�̂���]
	std::function<bool()> attackModeMainBodyRota =
		std::bind(&Medaman::AttackModeMainBodyRota, medaman);
	attackModeRotaSelector->AddNode(attackModeMainBodyRota);


	//���g����]
	std::function<bool()> attackModeAvatarRota =
		std::bind(&Medaman::AttackModeAvatarRota, medaman);
	attackModeRotaSelector->AddNode(attackModeAvatarRota);
}

void MedamanBehaviorTree::AttackSelectNode()
{
	//�U����ԓ����E�U���Z���N�g�m�[�h

	//�U�����e;�Ǐ]�����s����
	std::function<bool()> attackTypeTracking =
		std::bind(&Medaman::AttackTypeTracking, medaman);
	attackSelector->AddNode(attackTypeTracking);


	//�U�����e;������e�����s����
	std::function<bool()> attackTypeSuperGiantBullet =
		std::bind(&Medaman::AttackTypeSuperGiantBullet, medaman);
	attackSelector->AddNode(attackTypeSuperGiantBullet);


	//�U�����e;��]�����s����
	std::function<bool()> attackTypeRotate =
		std::bind(&Medaman::AttackTypeRotate, medaman);
	attackSelector->AddNode(attackTypeRotate);


	//�U�����e;���g�̓���������s����
	std::function<bool()> attackTypeAvatarBodyBlow =
		std::bind(&Medaman::AttackTypeAvatarBodyBlow, medaman);
	attackSelector->AddNode(attackTypeAvatarBodyBlow);


	//�U�����e;���g�K�g�����O�C�����s����
	std::function<bool()> attackTypeAvatarGatling =
		std::bind(&Medaman::AttackTypeAvatarGatling, medaman);
	attackSelector->AddNode(attackTypeAvatarGatling);


	//�U�����e;���g����e�����s����
	std::function<bool()> attackTypeAvatarGiantBullet =
		std::bind(&Medaman::AttackTypeAvatarGiantBullet, medaman);
	attackSelector->AddNode(attackTypeAvatarGiantBullet);
}

void MedamanBehaviorTree::WaitModeSequenceNode()
{
	//�ҋ@��ԃV�[�P���X�m�[�h

	//���Ԍo��
	std::function<bool()> waitModeCount =
		std::bind(&Medaman::WaitModeCount, medaman);
	waitModeSequencer->AddNode(waitModeCount);


	//�ҋ@��ԓ����Z���N�^�[
	std::function<bool()> waitModeActionSelect =
		std::bind(&Selector::Select, waitModeActionSelector.get());
	waitModeSequencer->AddNode(waitModeActionSelect);
	//�ҋ@��ԓ����Z���N�g�m�[�h
	WaitModeActionSelectNode();
}

void MedamanBehaviorTree::WaitModeActionSelectNode()
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
		std::bind(&Medaman::ReturnBasePosition, medaman);
	waitModeActionSelector->AddNode(returnBasePosition);
}

void MedamanBehaviorTree::WaitModeRotaSelectNode()
{
	//�ҋ@��ԓ����E��]�ΏۃZ���N�g�m�[�h

	//�{�̂���]
	std::function<bool()> waitModeMainBodyRota =
		std::bind(&Medaman::WaitModeMainBodyRota, medaman);
	waitModeRotaSelector->AddNode(waitModeMainBodyRota);


	//���g����]
	std::function<bool()> waitModeAvatarRota =
		std::bind(&Medaman::WaitModeAvatarRota, medaman);
	waitModeRotaSelector->AddNode(waitModeAvatarRota);
}

void MedamanBehaviorTree::DeadModeSelectNode()
{
	//���S��ԃZ���N�g�m�[�h

	//���S����
	std::function<bool()> deadExplosion =
		std::bind(&Medaman::DeadExplosion, medaman);
	deadModeSelector->AddNode(deadExplosion);


	//���S����
	std::function<bool()> deadFall =
		std::bind(&Medaman::DeadFall, medaman);
	deadModeSelector->AddNode(deadFall);
}
