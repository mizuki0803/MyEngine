#include "EnemyBullet.h"

EnemyBullet* EnemyBullet::Create(ObjModel* model, XMFLOAT3& position, XMFLOAT3& velocity)
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
	enemyBullet->SetModel(model);

	//���W���Z�b�g
	enemyBullet->SetPosition(position);

	//���x���Z�b�g
	enemyBullet->velocity = velocity;

	return enemyBullet;
}

bool EnemyBullet::Initialize()
{
	scale = { 0.1f, 0.1f, 0.1f };

	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	return true;
}

void EnemyBullet::Update()
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
