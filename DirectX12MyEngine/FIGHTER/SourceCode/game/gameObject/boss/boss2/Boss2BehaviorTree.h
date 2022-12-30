#pragma once
#include "Selector.h"
#include "Sequencer.h"
#include <d3dx12.h>

class Boss2;

/// <summary>
/// �X�e�[�W2�{�X�̍s���J�ڃr�w�C�r�A�c���[
/// </summary>
class Boss2BehaviorTree
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�{�X02�̍s���J�ڃr�w�C�r�A�c���[</returns>
	static Boss2BehaviorTree* Create(Boss2* boss2);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(Boss2* boss2);

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

private:
	//�{�X02
	Boss2* boss2 = nullptr;
	//�Z���N�^�[
	std::unique_ptr<Selector> topSelector;
	//�V�[�P���T�[
	std::unique_ptr<Sequencer> appearModeSequencer;
};