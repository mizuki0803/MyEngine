#include "EnemyFireBullet.h"
#include "ParticleEmitter.h"

EnemyFireBullet* EnemyFireBullet::Create(const Vector3& position, const Vector3& velocity, const float size)
{
	//�G�Ή��e�̃C���X�^���X�𐶐�
	EnemyFireBullet* enemyFireBullet = new EnemyFireBullet();
	if (enemyFireBullet == nullptr) {
		return nullptr;
	}

	// ������
	if (!enemyFireBullet->Initialize()) {
		delete enemyFireBullet;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	enemyFireBullet->position = position;
	//���x���Z�b�g
	enemyFireBullet->velocity = velocity;
	//�傫�����Z�b�g
	enemyFireBullet->scale = { size, size, size };
	//�����^�C�}�[���Z�b�g
	enemyFireBullet->deathTimer = lifeTime;

	return enemyFireBullet;
}

void EnemyFireBullet::Update()
{
	//�e�X�V
	EnemyBullet::Update();

	//����������Ԋu�̎���
	const int explosionInterval = 2;
	if (deathTimer % explosionInterval == 0) {
		//����
		//�D��̌����ɍ��킹��
		Vector3 pos = position;
		//�����_���ł��炷
		const Vector3 randPos = { 2, 2, 1 };
		pos.x += (float)((rand() % (int)randPos.x) - randPos.x / 2);
		pos.y += (float)((rand() % (int)randPos.y) - randPos.y / 2);
		pos.z += (float)((rand() % (int)randPos.z));

		//�Ή������o�p�p�[�e�B�N������
		const float size = scale.x * 1.5f;
		ParticleEmitter::GetInstance()->FireBall(GetWorldPos(), size, 1);
	}
}

void EnemyFireBullet::OnCollision()
{
	//���S������
	isDead = true;

	//�Ή������S���o�p�p�[�e�B�N������
	const float size = scale.x * 3;
	ParticleEmitter::GetInstance()->FireBall(GetWorldPos(), size, 1);
}
