#include "HomingBullet.h"
#include "Easing.h"
#include "Enemy.h"

HomingBullet* HomingBullet::Create(ObjModel* model, const Vector3& position, const Vector3& velocity, Enemy* enemy)
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

	//���f�����Z�b�g
	assert(model);
	homingBullet->model = model;

	//�e�̎�ނ��Z�b�g
	homingBullet->bulletType = BulletType::Homing;

	//���W���Z�b�g
	homingBullet->position = position;

	//���x���Z�b�g
	homingBullet->velocity = velocity;

	//�Ǐ]�Ώۂ̓G���Z�b�g
	homingBullet->enemy = enemy;

	return homingBullet;
}

bool HomingBullet::Initialize()
{
	//�F�����F�ɕύX
	XMFLOAT4 yellow = { 1.0f, 1.0f, 0.2f, 1 };
	color = yellow;

	//3D�I�u�W�F�N�g�̏�����
	if (!PlayerBullet::Initialize())
	{
		return false;
	}

	return true;
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
		velocity = Easing::Lerp(velocity, toPlayer, time) * speed;
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
	ObjObject3d::Update();
}
