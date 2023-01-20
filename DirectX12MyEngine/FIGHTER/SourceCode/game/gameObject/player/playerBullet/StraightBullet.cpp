#include "StraightBullet.h"
#include "ParticleEmitter.h"

StraightBullet* StraightBullet::Create(ObjModel* model, const Vector3& position, const Vector3& velocity, const float size)
{
	//���i�e�̃C���X�^���X�𐶐�
	StraightBullet* straightBullet = new StraightBullet();
	if (straightBullet == nullptr) {
		return nullptr;
	}

	// ������
	if (!straightBullet->Initialize()) {
		delete straightBullet;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	straightBullet->model = model;
	//�e�̎�ނ��Z�b�g
	straightBullet->bulletType = BulletType::Straight;
	//���W���Z�b�g
	straightBullet->position = position;
	//���x���Z�b�g
	straightBullet->velocity = velocity;
	//�傫�����Z�b�g
	straightBullet->scale = { size, size, size };
	//�_���[�W�ʂ��Z�b�g
	straightBullet->damageNum = bulletDamageNum;

	return straightBullet;
}

void StraightBullet::Update()
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
	PlayerBullet::Update();
}

void StraightBullet::OnCollision(const Vector3& subjectPos, float subjectSize, bool isToDamage)
{
	//���S������
	isDead = true;

	//�G�����ɉ��o���o�Ă��܂����Ƃ�����̂ŁA�G��Z���W����G�̑傫���������߂�
	Vector3 pos = GetWorldPos();
	pos.z = subjectPos.z - subjectSize;
	//�V���b�g���S���o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->ShotDead(pos, scale.x);
}

void StraightBullet::CollisionGround()
{
	//Y���W0�ȉ��łȂ���Δ�����
	if (position.y > 0) { return; }

	//���S
	isDead = true;
	//�V���b�g���S���o�p�p�[�e�B�N������
	Vector3 pos = GetWorldPos();
	pos.y = 0;
	ParticleEmitter::GetInstance()->ShotDead(pos, scale.x);
}
