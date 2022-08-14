#include "Selector.h"
#include <assert.h>

Selector* Selector::Create()
{
	//セレクターのインスタンスを生成
	Selector* selector = new Selector();
	if (selector == nullptr) {
		return nullptr;
	}

	return selector;
}

bool Selector::Select()
{
	//trueが帰ってきたら即終了
	for (int i = 0; i < (signed)nodes.size(); i++) {
		if (nodes[i]()) {
			return true;
		}
	}

	return false;
}

void Selector::AddNode(bool func())
{
	//子ノードリストに登録
	nodes.push_back(func);
}
