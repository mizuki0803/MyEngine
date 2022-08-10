#include "BossHead.h"

BossHead* BossHead::Create(ObjModel* model, ObjObject3d* parent)
{
	//�{�X(��)�̃C���X�^���X�𐶐�
	BossHead* bossHead = new BossHead();
	if (bossHead == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossHead->model = model;

	// ������
	if (!bossHead->Initialize()) {
		delete bossHead;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	bossHead->parent = parent;

	return bossHead;
}
