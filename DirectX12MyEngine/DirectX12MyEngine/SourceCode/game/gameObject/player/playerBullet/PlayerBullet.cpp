#include "PlayerBullet.h"

bool PlayerBullet::Initialize()
{
	scale = { 0.5f, 0.5f, 0.5f };

	//3D�I�u�W�F�N�g�̏�����
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	return true;
}

void PlayerBullet::Update()
{
	//�n�ʂɐG�ꂽ�Ƃ�
	CollisionGround();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void PlayerBullet::OnCollision()
{
	//���S������
	isDead = true;
}

Vector3 PlayerBullet::GetWorldPos()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���s�ړ��������擾
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void PlayerBullet::CollisionGround()
{
	//Y���W0�ȉ��łȂ���Δ�����
	if (position.y > 0) { return; }

	//���S
	isDead = true;
}