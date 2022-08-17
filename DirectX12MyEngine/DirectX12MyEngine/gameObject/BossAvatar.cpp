#include "BossAvatar.h"

BossAvatar* BossAvatar::Create(ObjModel* model, ObjObject3d* parent)
{
	//�{�X(���g)�̃C���X�^���X�𐶐�
	BossAvatar* bossAvatar = new BossAvatar();
	if (bossAvatar == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossAvatar->model = model;

	// ������
	if (!bossAvatar->Initialize()) {
		delete bossAvatar;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	bossAvatar->parent = parent;

	return bossAvatar;
}
