#include "Enemy.h"

Enemy* Enemy::Create(ObjModel* model, const XMFLOAT3& position, const XMFLOAT3& velocity)
{
	//�G�̃C���X�^���X�𐶐�
	Enemy* enemy = new Enemy();
	if (enemy == nullptr) {
		return nullptr;
	}

	// ������
	if (!enemy->Initialize()) {
		delete enemy;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	enemy->SetModel(model);

	//���W���Z�b�g
	enemy->SetPosition(position);

	//���x���Z�b�g
	enemy->velocity = velocity;

	return enemy;
}

bool Enemy::Initialize()
{
	//3D�I�u�W�F�N�g�̏�����
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	return true;
}

void Enemy::Update()
{
	//�ړ�
	Move();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();
}

void Enemy::Move()
{
	switch (phase)
	{
	case Phase::Previous:
	default:
		//�O�i����
		position.x += velocity.x;
		position.y += velocity.y;
		position.z += velocity.z;
		//���̈ʒu�܂őO�i��������
		if (position.z < 0.0f) {
			phase = Phase::Back;
		}

		break;
	case Phase::Back:
		//��ނ���
		position.x -= velocity.x;
		position.y -= velocity.y;
		position.z -= velocity.z;
		//���̈ʒu�܂Ō�ނ�����O�i
		if (position.z > 100.0f) {
			phase = Phase::Previous;
		}

		break;
	}
}
