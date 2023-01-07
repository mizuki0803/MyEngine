#pragma once
#include "Selector.h"
#include "Sequencer.h"
#include <d3dx12.h>

class Galaxy;

/// <summary>
/// �X�e�[�W2�{�X�̍s���J�ڃr�w�C�r�A�c���[
/// </summary>
class GalaxyBehaviorTree
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�M�����N�V�[�̍s���J�ڃr�w�C�r�A�c���[</returns>
	static GalaxyBehaviorTree* Create(Galaxy* galaxy);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(Galaxy* galaxy);

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
	/// �U���Z���N�g�m�[�h
	/// </summary>
	void AttackSelectNode();

private:
	//�M�����N�V�[
	Galaxy* galaxy = nullptr;
	//�Z���N�^�[
	std::unique_ptr<Selector> topSelector;
	//�V�[�P���T�[
	std::unique_ptr<Sequencer> appearModeSequencer;
	//�V�[�P���T�[
	std::unique_ptr<Sequencer> attackModeSequencer;
	//�Z���N�^�[
	std::unique_ptr<Selector> attackTypeSelector;
	//�Z���N�^�[
	std::unique_ptr<Selector> attackSelector;
};