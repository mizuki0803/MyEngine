#include "BossAvatarType02.h"

BossAvatarType02* BossAvatarType02::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//�{�X(���g�F�^�C�v02)�̃C���X�^���X�𐶐�
	BossAvatarType02* bossAvatarType02 = new BossAvatarType02();
	if (bossAvatarType02 == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossAvatarType02->model = model;

	// ������
	if (!bossAvatarType02->Initialize()) {
		delete bossAvatarType02;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	bossAvatarType02->parent = parent;

	//���W���Z�b�g
	bossAvatarType02->position = position;

	//�e�q�֌W��ł̒�ʒu���Z�b�g
	bossAvatarType02->fixedPos = position;

	return bossAvatarType02;
}

void BossAvatarType02::AttackTypeBLockon()
{
	//�^�C�}�[���X�V
	const float backTime = 150;
	attackBTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackBTimer >= backTime) {
		attackBPhase = AttackTypeBPhase::Shot;

		//��΂����x���m�肳����
		attackBVelocity = attackBLockonPos - GetWorldPos();
		const float moveSpeed = 1.0f;
		attackBVelocity = attackBVelocity.normalize() * moveSpeed;

		//�^�C�}�[������
		attackBTimer = 0;
	}
}
