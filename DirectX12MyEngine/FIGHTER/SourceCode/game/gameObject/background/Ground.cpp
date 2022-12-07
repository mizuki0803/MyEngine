#include "Ground.h"

Ground* Ground::Create(ObjModel* model)
{
	//�n�ʂ̃C���X�^���X�𐶐�
	Ground* ground = new Ground();
	if (ground == nullptr) {
		return nullptr;
	}

	// ������
	if (!ground->Initialize()) {
		delete ground;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	ground->model = model;

	//�傫�����Z�b�g
	ground->scale = { 20, 20, 20 };

	//�V���h�E�}�b�v�ŉe��t����
	ground->isShadowMap = true;

	return ground;
}
