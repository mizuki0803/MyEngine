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

	//���W���Z�b�g
	ground->position = { 0, -15, 0 };

	//�傫�����Z�b�g
	ground->scale = { 10, 10, 10 };

	return ground;
}
