#include "Cannon.h"
#include "Player.h"

Cannon* Cannon::Create(ObjModel* model, const Vector3& position)
{
	//�G�̃C���X�^���X�𐶐�
	Cannon* cannon = new Cannon();
	if (cannon == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	cannon->model = model;

	// ������
	if (!cannon->Initialize()) {
		delete cannon;
		assert(0);
		return nullptr;
	}


	//���W���Z�b�g
	cannon->position = position;

	return cannon;
}

void Cannon::Update()
{
	//���˃^�C�}�[�J�E���g�_�E��
	--fireTimer;
	if (fireTimer <= 0) {
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		fireTimer = fireInterval;
	}

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();
}

void Cannon::FrontOfScreenDelete()
{
	//���W�����@����O(��ʊO��O)�܂ōs������폜
	const float flontOfScreenDiffence = 100;
	if (GetWorldPos().z <= player->GetWorldPos().z - flontOfScreenDiffence) {
		isDead = true;
	}
}
