#include "BossDeadEffect.h"
#include "ParticleEmitter.h"

BossDeadEffect* BossDeadEffect::Create(const Vector3& deadPos, const float explosionSize, const bool isBlackSmoke, const bool isGround)
{
	//�{�X���S��G�t�F�N�g�̃C���X�^���X�𐶐�
	BossDeadEffect* bossDeadEffect = new BossDeadEffect();
	if (bossDeadEffect == nullptr) {
		return nullptr;
	}

	//�{�X���S���W���Z�b�g
	bossDeadEffect->deadPos = deadPos;
	//�����̑傫�����Z�b�g
	bossDeadEffect->explosionSize = explosionSize;
	//�������o�����Z�b�g
	bossDeadEffect->isBlackSmoke = isBlackSmoke;
	//�n�ʗp�̔��������邩���Z�b�g
	bossDeadEffect->isGround = isGround;

	return bossDeadEffect;
}

void BossDeadEffect::Update()
{
	//�^�C�}�[�X�V
	timer++;

	//����
	Explosion();

	//�������o����ԂłȂ���Δ�����
	if (!isBlackSmoke) { return; }

	//����
	BlackSmoke();
}

void BossDeadEffect::Explosion()
{
	//���ɔ����I���t���O�������Ă����甲����
	if (isExplosionEnd) { return; }

	//���t���[���o���Ƒ����̂ŊԊu��ݒ�
	const int explosionInterval = 15;
	//�w�肵���Ԋu�ȊO�Ȃ甲����
	if (timer % explosionInterval != 0) { return; }

	//�������o�p�p�[�e�B�N������
	Vector3 explosionPos = deadPos;
	const float distance = 15.0f;
	explosionPos.x += (float)rand() / RAND_MAX * distance - distance / 2.0f;
	if (!isGround) { explosionPos.y += (float)rand() / RAND_MAX * distance - distance / 2.0f; }
	explosionPos.z += (float)rand() / RAND_MAX * distance - distance / 2.0f;
	ParticleEmitter::GetInstance()->BossDeadExplosion(explosionPos, explosionSize, isGround);

	//�������o�񐔍X�V
	explosionCount++;
	//�������w�肵���񐔔���������I���t���O�𗧂Ă�
	const int explosionNum = 5;
	if (explosionCount >= explosionNum) {
		isExplosionEnd = true;
	}
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
