#include "BossMainBody.h"

BossMainBody* BossMainBody::Create(ObjModel* model, const Vector3& position)
{
	//�{�X(�{��)�̃C���X�^���X�𐶐�
	BossMainBody* bossMainBody = new BossMainBody();
	if (bossMainBody == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossMainBody->model = model;

	// ������
	if (!bossMainBody->Initialize()) {
		delete bossMainBody;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	bossMainBody->position = position;

	return bossMainBody;
}
