#include "PlayerBullet.h"

bool PlayerBullet::isGroundMode = true;

void PlayerBullet::Update()
{
	//�n�ʂ���s�����s���ꍇ�̂�
	if (isGroundMode) {
		//�n�ʂɐG�ꂽ�Ƃ��̋���
		CollisionGround();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}
