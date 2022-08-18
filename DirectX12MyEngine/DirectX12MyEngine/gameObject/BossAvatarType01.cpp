#include "BossAvatarType01.h"

BossAvatarType01* BossAvatarType01::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//�{�X(���g�F�^�C�v01)�̃C���X�^���X�𐶐�
	BossAvatarType01* bossAvatarType01 = new BossAvatarType01();
	if (bossAvatarType01 == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossAvatarType01->model = model;

	// ������
	if (!bossAvatarType01->Initialize()) {
		delete bossAvatarType01;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	bossAvatarType01->parent = parent;

	//���W���Z�b�g
	bossAvatarType01->position = position;

	return bossAvatarType01;
}

void BossAvatarType01::Otamesi()
{
	position.x -= moveSpeed;
}

void BossAvatarType01::Otamesi2()
{
	position.x += moveSpeed;
}

void BossAvatarType01::Otamesi3()
{
	position.y += moveSpeed;
}

void BossAvatarType01::Otamesi4()
{
	position.y -= moveSpeed;
}
