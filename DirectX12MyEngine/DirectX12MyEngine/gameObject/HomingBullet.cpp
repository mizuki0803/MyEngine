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

	//���W���Z�b�g
	homingBullet->position = position;

	//���x���Z�b�g
	homingBullet->velocity = velocity;

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
		const float maxTime = 180;
		float time = homingTimer / maxTime;
		velocity = Easing::Lerp(velocity, toPlayer, time) * 0.3f;
		homingTimer++;
		homingTimer = min(homingTimer, maxTime);
	}

	position += velocity;


	//�i�s�����������悤�ɂ���
	rotation.y = XMConvertToDegrees(std::atan2(velocity.x, velocity.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 velocityZ = MatrixTransformDirection(velocity, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-velocityZ.y, velocityZ.z));

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();
}
