#include "CannonEnemy.h"
#include "Player.h"

CannonEnemy* CannonEnemy::Create(ObjModel* model, const Vector3& position)
{
	//�G�̃C���X�^���X�𐶐�
	CannonEnemy* cannon = new CannonEnemy();
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

void CannonEnemy::Update()
{
	//���˃^�C�}�[�J�E���g�_�E��
	--fireTimer;
	if (fireTimer <= 0) {
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		fireTimer = fireInterval;
	}

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();
}
