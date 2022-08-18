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
	void MakeTree(Boss* boss);

private:
	//�Z���N�^�[
	std::unique_ptr<Selector> topSelector;
	//�Z���N�^�[
	std::unique_ptr<Selector> selector1or2or3or4;
};