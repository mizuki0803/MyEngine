#include "PlayerBullet.h"

void PlayerBullet::Update()
{
	//�n�ʂɐG�ꂽ�Ƃ�
	CollisionGround();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
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
