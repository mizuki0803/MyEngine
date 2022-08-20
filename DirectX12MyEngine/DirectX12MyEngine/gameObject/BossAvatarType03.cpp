#include "BossAvatarType03.h"

BossAvatarType03* BossAvatarType03::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//�{�X(���g�F�^�C�v03)�̃C���X�^���X�𐶐�
	BossAvatarType03* bossAvatarType03 = new BossAvatarType03();
	if (bossAvatarType03 == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossAvatarType03->model = model;

	// ������
	if (!bossAvatarType03->Initialize()) {
		delete bossAvatarType03;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	bossAvatarType03->parent = parent;

	//���W���Z�b�g
	bossAvatarType03->position = position;

	return bossAvatarType03;
}
