#include "PlayerBullet.h"

PlayerBullet* PlayerBullet::Create(ObjModel* model, XMFLOAT3 position)
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
	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();
}