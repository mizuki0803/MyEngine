#include "Enemy.h"
#include "Player.h"

Enemy* Enemy::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
{
	//�G�̃C���X�^���X�𐶐�
	Enemy* enemy = new Enemy();
	if (enemy == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	enemy->model = model;

	// ������
	if (!enemy->Initialize()) {
		delete enemy;
		assert(0);
		return nullptr;
	}


	//���W���Z�b�g
	enemy->position = position;

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

void Enemy::OnCollision()
{
}

void Enemy::PreviousPhaseInit()
{
	//���˃^�C�}�[��������
	fireTimer = fireInterval;
}

Vector3 Enemy::GetWorldPos()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���s�ړ��������擾
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
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
		if (position.z > 50.0f) {
			phase = Phase::Previous;
		}

		break;
	}
}

void Enemy::Fire()
{
	//�e�̑��x��ݒ�
	const float bulletSpeed = 0.5f;

	Vector3 playerPos = player->GetWorldPos();
	Vector3 enemyPos = GetWorldPos();
	Vector3 DifferenceVec = playerPos - enemyPos;
	DifferenceVec.normalize();
	Vector3 velocity = DifferenceVec * bulletSpeed;

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(model, position, velocity));
	enemyBullets.push_back(std::move(newBullet));
}