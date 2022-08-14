#include "Sequencer.h"

Sequencer* Sequencer::Create()
{
	//シーケンサーのインスタンスを生成
	Sequencer* sequencer = new Sequencer();
	if (sequencer == nullptr) {
		return nullptr;
	}

	return sequencer;
}

bool Sequencer::Sequence()
{
	//falseが帰ってきたら即終了
	for (int i = 0; i < (signed)nodes.size(); i++) {
		if (!nodes[i]()) {
			return false;
		}
	}

	return true;
}

void Sequencer::AddNode(bool func())
{
    //子ノードリストに登録
    nodes.push_back(func);
}
