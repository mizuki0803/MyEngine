#include "Sequencer.h"

Sequencer* Sequencer::Create()
{
	//�V�[�P���T�[�̃C���X�^���X�𐶐�
	Sequencer* sequencer = new Sequencer();
	if (sequencer == nullptr) {
		return nullptr;
	}

	return sequencer;
}

bool Sequencer::Sequence()
{
	//false���A���Ă����瑦�I��
	for (int i = 0; i < (signed)nodes.size(); i++) {
		if (!nodes[i]()) {
			return false;
		}
	}

	return true;
}

void Sequencer::AddNode(bool func())
{
    //�q�m�[�h���X�g�ɓo�^
    nodes.push_back(func);
}
