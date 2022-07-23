#include "StraightBullet.h"

StraightBullet* StraightBullet::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
{
	//���@�e�̃C���X�^���X�𐶐�
	StraightBullet* straightBullet = new StraightBullet();
	if (straightBullet == nullptr) {
		return nullptr;
	}

	// ������
	if (!straightBullet->Initialize()) {
		delete straightBullet;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	straightBullet->model = model;

	//���W���Z�b�g
	straightBullet->position = position;

	//���x���Z�b�g
	straightBullet->velocity = velocity;

	return straightBullet;
}

void StraightBullet::Update()
{
	//���W���ړ�
	position += velocity;

	//���Ԍo�߂Ŏ��S
	if (--deathTimer <= 0) {
		isDead = true;
	}

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();
}
