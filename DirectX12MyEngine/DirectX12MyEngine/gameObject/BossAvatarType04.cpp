#include "BossAvatarType04.h"

BossAvatarType04* BossAvatarType04::Create(ObjModel* model, ObjObject3d* parent, const Vector3& position)
{
	//�{�X(���g�F�^�C�v04)�̃C���X�^���X�𐶐�
	BossAvatarType04* bossAvatarType04 = new BossAvatarType04();
	if (bossAvatarType04 == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossAvatarType04->model = model;

	// ������
	if (!bossAvatarType04->Initialize()) {
		delete bossAvatarType04;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	bossAvatarType04->parent = parent;

	//���W���Z�b�g
	bossAvatarType04->position = position;

	//�e�q�֌W��ł̒�ʒu���Z�b�g
	bossAvatarType04->fixedPos = position;

	return bossAvatarType04;
}

void BossAvatarType04::AttackTypeBLockon()
{
	//�^�C�}�[���X�V
	const float backTime = 180;
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
