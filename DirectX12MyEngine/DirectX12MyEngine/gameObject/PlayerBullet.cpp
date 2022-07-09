#include "PlayerBullet.h"

PlayerBullet* PlayerBullet::Create(ObjModel* model, const XMFLOAT3& position, const XMFLOAT3& velocity)
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
	playerBullet->SetModel(model);

	//���W���Z�b�g
	playerBullet->SetPosition(position);

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
	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;

	//���Ԍo�߂Ŏ��S
	if (--deathTimer <= 0) {
		isDead = true;
	}

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();
}
