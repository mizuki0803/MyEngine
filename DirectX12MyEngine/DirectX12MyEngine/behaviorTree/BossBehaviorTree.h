#pragma once
#include "Selector.h"
#include "Sequencer.h"
#include <d3dx12.h>

class Boss;

/// <summary>
/// �{�X�̍s���J�ڃr�w�C�r�A�c���[
/// </summary>
class BossBehaviorTree
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�{�X�̍s���J�ڃr�w�C�r�A�c���[</returns>
	static BossBehaviorTree* Create(Boss* boss);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize(Boss* boss);

	/// <summary>
	/// �s���J��
	/// </summary>
	void Root();

private: //�����o�֐�
	/// <summary>
	/// �؍\���̍쐬
	/// </summary>
	void MakeTree();

	/// <summary>
	/// �U����ԃV�[�P���X�m�[�h
	/// </summary>
	void AttackModeSequenceNode();

	/// <summary>
	/// �U�����e�Z���N�g�m�[�h
	/// </summary>
	void AttackTypeSelectNode();

	/// <summary>
	/// �U����ԓ����Z���N�g�m�[�h
	/// </summary>
	void AttackModeActionSelectNode();

	/// <summary>
	/// �U����ԓ����E��]�ΏۃZ���N�g�m�[�h
	/// </summary>
	void AttackModeRotaSelectNode();

	/// <summary>
	/// �U����ԓ����E�U���Z���N�g�m�[�h
	/// </summary>
	void AttackSelectNode();

	/// <summary>
	/// �ҋ@��ԃV�[�P���X�m�[�h
	/// </summary>
	void WaitModeSequenceNode();

	/// <summary>
	/// �ҋ@��ԓ����Z���N�g�m�[�h
	/// </summary>
	void WaitModeActionSelectNode();

	/// <summary>
	/// �ҋ@��ԓ����E��]�ΏۃZ���N�g�m�[�h
	/// </summary>
	void WaitModeRotaSelectNode();

private:
	//�{�X
	Boss* boss = nullptr;
	//�Z���N�^�[
	std::unique_ptr<Selector> topSelector;
	//�V�[�P���T�[
	std::unique_ptr<Sequencer> attackModeSequencer;
	//�V�[�P���T�[
	std::unique_ptr<Sequencer> waitModeSequencer;
	//�Z���N�^�[
	std::unique_ptr<Selector> attackTypeSelector;
	//�Z���N�^�[
	std::unique_ptr<Selector> attackModeActionSelector;
	//�Z���N�^�[
	std::unique_ptr<Selector> attackModeRotaSelector;
	//�Z���N�^�[
	std::unique_ptr<Selector> attackSelector;
	//�Z���N�^�[
	std::unique_ptr<Selector> waitModeActionSelector;
	//�Z���N�^�[
	std::unique_ptr<Selector> waitModeRotaSelector;
};