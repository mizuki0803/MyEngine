#pragma once
#include "Selector.h"
#include "Sequencer.h"
#include <d3dx12.h>

class Medaman;

/// <summary>
/// ���_�}�[���̍s���J�ڃr�w�C�r�A�c���[
/// </summary>
class MedamanBehaviorTree
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>���_�}�[���̍s���J�ڃr�w�C�r�A�c���[</returns>
	static MedamanBehaviorTree* Create(Medaman* medaman);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize(Medaman* medaman);

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
	/// �o���ԃV�[�P���X�m�[�h
	/// </summary>
	void AppearModeSequenceNode();

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

	/// <summary>
	/// ���S��ԃZ���N�g�m�[�h
	/// </summary>
	void DeadModeSelectNode();

private:
	//���_�}�[��
	Medaman* medaman = nullptr;
	//�Z���N�^�[
	std::unique_ptr<Selector> topSelector;
	//�V�[�P���T�[
	std::unique_ptr<Sequencer> appearModeSequencer;
	//�V�[�P���T�[
	std::unique_ptr<Sequencer> attackModeSequencer;
	//�V�[�P���T�[
	std::unique_ptr<Sequencer> waitModeSequencer;
	//�Z���N�^�[
	std::unique_ptr<Selector> deadModeSelector;
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