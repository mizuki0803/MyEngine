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
	scale = { 0.3f, 0.3f, 0.3f };

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

void PlayerBullet::OnCollision()
{
	//���S������
	isDead = true;
}

Vector3 PlayerBullet::GetWorldPos()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���s�ړ��������擾
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}
