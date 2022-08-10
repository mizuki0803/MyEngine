#include "BossBody.h"

BossBody* BossBody::Create(ObjModel* model, const Vector3& position)
{
	//�{�X(��)�̃C���X�^���X�𐶐�
	BossBody* bossBody = new BossBody();
	if (bossBody == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossBody->model = model;

	// ������
	if (!bossBody->Initialize()) {
		delete bossBody;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	bossBody->position = position;

	return bossBody;
}
