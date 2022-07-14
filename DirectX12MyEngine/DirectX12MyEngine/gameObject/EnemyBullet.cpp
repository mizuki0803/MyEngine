#include "EnemyBullet.h"

EnemyBullet* EnemyBullet::Create(ObjModel* model, Vector3& position, Vector3& velocity)
{
	//�e�e�̃C���X�^���X�𐶐�
	EnemyBullet* enemyBullet = new EnemyBullet();
	if (enemyBullet == nullptr) {
		return nullptr;
	}

	// ������
	if (!enemyBullet->Initialize()) {
		delete enemyBullet;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	enemyBullet->model = model;

	//���W���Z�b�g
	enemyBullet->position = position;

	//���x���Z�b�g
	enemyBullet->velocity = velocity;

	return enemyBullet;
}

bool EnemyBullet::Initialize()
{
	scale = { 0.5f, 0.5f, 0.5f };

	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	return true;
}

void EnemyBullet::Update()
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

void EnemyBullet::OnCollision()
{
	//���S������
	isDead = true;
}

Vector3 EnemyBullet::GetWorldPos()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���s�ړ��������擾
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}
