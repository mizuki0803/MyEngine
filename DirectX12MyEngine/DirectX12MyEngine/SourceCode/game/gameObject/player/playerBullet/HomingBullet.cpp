#include "HomingBullet.h"
#include "Easing.h"
#include "Enemy.h"
#include "ParticleEmitter.h"

const float HomingBullet::blastSize = 16.0f;

HomingBullet* HomingBullet::Create(const Vector3& position, const Vector3& velocity, const float size, Enemy* enemy)
{
	//�z�[�~���O�e�̃C���X�^���X�𐶐�
	HomingBullet* homingBullet = new HomingBullet();
	if (homingBullet == nullptr) {
		return nullptr;
	}

	// ������
	if (!homingBullet->Initialize()) {
		delete homingBullet;
		assert(0);
		return nullptr;
	}

	//�e�̎�ނ��Z�b�g
	homingBullet->bulletType = BulletType::Homing;

	//���W���Z�b�g
	homingBullet->position = position;

	//���x���Z�b�g
	homingBullet->velocity = velocity;

	//�T�C�Y���Z�b�g
	homingBullet->scale = { size, size, size };

	//�Ǐ]�Ώۂ̓G���Z�b�g
	homingBullet->enemy = enemy;

	return homingBullet;
}

void HomingBullet::Update()
{
	//���W���ړ�
	if (enemy) {
		Vector3 toPlayer = enemy->GetWorldPos() - GetWorldPos();
		toPlayer.normalize();
		velocity.normalize();
		const float maxTime = 120;
		homingTimer++;
		homingTimer = min(homingTimer, maxTime);
		const float time = homingTimer / maxTime;
		const float speed = 1.5f;
		velocity = Easing::LerpVec3(velocity, toPlayer, time) * speed;
	}
	position += velocity;


	//���Ԍo�߂Ŏ��S
	if (--deathTimer <= 0) {
		isDead = true;
	}

	//�i�s�����������悤�ɂ���
	rotation.y = XMConvertToDegrees(std::atan2(velocity.x, velocity.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 velocityZ = MatrixTransformDirection(velocity, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-velocityZ.y, velocityZ.z));

	//3D�I�u�W�F�N�g�̍X�V
	PlayerBullet::Update();

	//�`���[�W�V���b�g���o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->ChargeShot(GetWorldPos(), scale.x);
}

void HomingBullet::OnCollision(float subjectSize)
{
	//���S������
	isDead = true;

	//�`���[�W�V���b�g���S���o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->ChargeShotDead(GetWorldPos(), blastSize);
}

void HomingBullet::CollisionGround()
{
	//Y���W0�ȉ��łȂ���Δ�����
	if (position.y > 0) { return; }

	//���S
	isDead = true;
	//�`���[�W�V���b�g���S���o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->ChargeShotDead(GetWorldPos(), blastSize);
}
