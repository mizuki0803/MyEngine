#include "BossChargeBullet.h"
#include "ParticleEmitter.h"

BossChargeBullet* BossChargeBullet::Create(const Vector3& position, const Vector3& velocity, const float size)
{
	//�{�X�`���[�W�e�̃C���X�^���X�𐶐�
	BossChargeBullet* bossChargeBullet = new BossChargeBullet();
	if (bossChargeBullet == nullptr) {
		return nullptr;
	}

	// ������
	if (!bossChargeBullet->Initialize()) {
		delete bossChargeBullet;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	bossChargeBullet->position = position;

	//���x���Z�b�g
	bossChargeBullet->velocity = velocity;

	//�傫�����Z�b�g
	bossChargeBullet->scale = { size, size, size };

	return bossChargeBullet;
}

void BossChargeBullet::Update()
{
	//�e�X�V
	EnemyBullet::Update();

	//�{�X�`���[�W�V���b�g���o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->BossChargeShot(GetWorldPos(), scale.x);
}

void BossChargeBullet::OnCollision()
{
	//���S������
	isDead = true;

	//�{�X�`���[�W�V���b�g���S���o�p�p�[�e�B�N������
	const float blastSize = scale.x * 3;
	ParticleEmitter::GetInstance()->BossChargeShotDead(GetWorldPos(), blastSize);
}
