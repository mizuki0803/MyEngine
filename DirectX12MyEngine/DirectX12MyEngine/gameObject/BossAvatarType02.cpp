#include "BossAvatarType02.h"

BossAvatarType02* BossAvatarType02::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//�{�X(���g�F�^�C�v02)�̃C���X�^���X�𐶐�
	BossAvatarType02* bossAvatarType02 = new BossAvatarType02();
	if (bossAvatarType02 == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossAvatarType02->model = model;

	// ������
	if (!bossAvatarType02->Initialize()) {
		delete bossAvatarType02;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	bossAvatarType02->parent = parent;

	//���W���Z�b�g
	bossAvatarType02->position = position;

	return bossAvatarType02;
}
