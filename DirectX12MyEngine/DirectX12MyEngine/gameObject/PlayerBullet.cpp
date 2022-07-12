#include "PlayerBullet.h"

PlayerBullet* PlayerBullet::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
{
	//���@�e�̃C���X�^���X�𐶐�
	PlayerBullet* playerBullet = new PlayerBullet();
	if (playerBullet == nullptr) {
		return nullptr;
	}

	// ������
	if (!playerBullet->Initialize()) {
		delete playerBullet;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	playerBullet->model = model;

	//���W���Z�b�g
	playerBullet->position = position;

	//���x���Z�b�g
	playerBullet->velocity = velocity;

	return playerBullet;
}

bool PlayerBullet::Initialize()
{
	scale = { 0.1f, 0.1f, 0.1f };

	//3D�I�u�W�F�N�g�̏�����
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	return true;
}

void PlayerBullet::Update()
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
