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
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�{�X�̍s���J�ڃr�w�C�r�A�c���[</returns>
	static BossBehaviorTree* Create(Boss* boss);

	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize(Boss* boss);


	void Root();

private:
	//�Z���N�^�[
	std::unique_ptr<Selector> selector;
	//�V�[�P���T�[
	std::unique_ptr<Sequencer> sequencer;
};