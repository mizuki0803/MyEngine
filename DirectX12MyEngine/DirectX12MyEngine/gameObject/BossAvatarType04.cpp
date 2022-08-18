#include "BossAvatarType04.h"

BossAvatarType04* BossAvatarType04::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//�{�X(���g�F�^�C�v04)�̃C���X�^���X�𐶐�
	BossAvatarType04* bossAvatarType04 = new BossAvatarType04();
	if (bossAvatarType04 == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossAvatarType04->model = model;

	// ������
	if (!bossAvatarType04->Initialize()) {
		delete bossAvatarType04;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	bossAvatarType04->parent = parent;

	//���W���Z�b�g
	bossAvatarType04->position = position;

	return bossAvatarType04;
}

void BossAvatarType04::Otamesi()
{
	position.y += moveSpeed;
}

void BossAvatarType04::Otamesi2()
{
	position.y -= moveSpeed;
}

void BossAvatarType04::Otamesi3()
{
	position.x += moveSpeed;
}

void BossAvatarType04::Otamesi4()
{
	position.x -= moveSpeed;
}
