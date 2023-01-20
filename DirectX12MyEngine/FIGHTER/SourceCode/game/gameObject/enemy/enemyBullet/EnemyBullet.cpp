#include "EnemyBullet.h"

EnemyBullet* EnemyBullet::Create(ObjModel* model, const Vector3& position, const Vector3& velocity, const float scale)
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

	//�傫�����Z�b�g
	enemyBullet->scale = { scale, scale, scale };

	return enemyBullet;
}

void EnemyBullet::Update()
{
	//���W���ړ�
	position += velocity;

	//���Ԍo�߂Ŏ��S
	if (--deathTimer <= 0) {
		isDead = true;
	}

	//�i�s�����������悤�ɂ���
	rotation = Vector3::VelocityRotate(velocity);

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();
}

void EnemyBullet::OnCollision()
{
	//���S������
	isDead = true;
}
