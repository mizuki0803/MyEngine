#include "Enemy.h"

Enemy* Enemy::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
{
	//�G�̃C���X�^���X�𐶐�
	Enemy* enemy = new Enemy();
	if (enemy == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	enemy->SetModel(model);

	// ������
	if (!enemy->Initialize()) {
		delete enemy;
		assert(0);
		return nullptr;
	}


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

	//�e����
	//Fire();

	PreviousPhaseInit();

	return true;
}

void Enemy::Update()
{
	//���S�����e�̍폜
	enemyBullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->GetIsDead();
		});

	//�ړ�
	Move();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();

	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		bullet->Update();
	}
}

void Enemy::Draw()
{
	ObjObject3d::Draw();

	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		bullet->Draw();
	}
}

void Enemy::PreviousPhaseInit()
{
	//���˃^�C�}�[��������
	fireTimer = fireInterval;
}

void Enemy::Move()
{
	switch (phase)
	{
	case Phase::Previous:
	default:
		//�O�i����
		position -= velocity;

		//���̈ʒu�܂őO�i��������
		if (position.z < 0.0f) {
			phase = Phase::Back;
		}

		//���˃^�C�}�[�J�E���g�_�E��
		--fireTimer;
		if (fireTimer <= 0) {
			//�e�𔭎�
			Fire();
			//���˃^�C�}�[��������
			fireTimer = fireInterval;
		}

		break;
	case Phase::Back:
		//��ނ���
		position += velocity;
		//���̈ʒu�܂Ō�ނ�����O�i
		if (position.z > 100.0f) {
			phase = Phase::Previous;
		}

		break;
	}
}

void Enemy::Fire()
{
	//�e�̑��x��ݒ�
	const float bulletSpeed = 0.5f;
	Vector3 velocity(0, 0, -bulletSpeed);

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(model, position, velocity));
	enemyBullets.push_back(std::move(newBullet));
}
