#include "BossDeadEffect.h"
#include "ParticleEmitter.h"

BossDeadEffect* BossDeadEffect::Create(const Vector3& deadPos)
{
	//�e�e�̃C���X�^���X�𐶐�
	BossDeadEffect* bossDeadEffect = new BossDeadEffect();
	if (bossDeadEffect == nullptr) {
		return nullptr;
	}

	//�{�X���S���W���Z�b�g
	bossDeadEffect->deadPos = deadPos;

	return bossDeadEffect;
}

void BossDeadEffect::Update()
{
	//�^�C�}�[�X�V
	timer++;

	//����
	Explosion();
	//����
	BlackSmoke();
}

void BossDeadEffect::Explosion()
{
	//���ɔ����񐔂��\���Ȃ甲����
	const int explosionNum = 7;
	if (explosionCount >= explosionNum) { return; }

	//���t���[���o���Ƒ����̂ŊԊu��ݒ�
	const int explosionInterval = 12;
	//�w�肵���Ԋu�ȊO�Ȃ甲����
	if (timer % explosionInterval != 0) { return; }

	//�������o�p�p�[�e�B�N������
	Vector3 explosionPos = deadPos;
	const float distance = 15.0f;
	explosionPos.x += (float)rand() / RAND_MAX * distance - distance / 2.0f;
	explosionPos.z += (float)rand() / RAND_MAX * distance - distance / 2.0f;
	ParticleEmitter::GetInstance()->BossDeadExplosion(explosionPos);

	//�������o�񐔍X�V
	explosionCount++;
}

void BossDeadEffect::BlackSmoke()
{
	//���S���Ă��獕�����o���܂ł̎���
	const int smokeStartTime = 40;
	//�^�C�}�[���������o���܂ł̎��Ԉȉ��Ȃ甲����
	if (timer < smokeStartTime) { return; }

	//���t���[���o���Ƒ����̂ŊԊu��ݒ�
	const int smokeInterval = 5;
	//�w�肵���Ԋu�ȊO�Ȃ甲����
	if (timer % smokeInterval != 0) { return; }

	//���S���W�ɍ����p�[�e�B�N������
	const float smokeSize = 10.0f;
	const int smokeAliveTime = 200;
	ParticleEmitter::GetInstance()->BlackSmoke(deadPos, smokeSize, smokeAliveTime);
}
