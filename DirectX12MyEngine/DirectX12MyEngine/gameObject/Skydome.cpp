#include "Skydome.h"

Skydome* Skydome::Create(ObjModel* model)
{
	//�V���̃C���X�^���X�𐶐�
	Skydome* skydome = new Skydome();
	if (skydome == nullptr) {
		return nullptr;
	}

	// ������
	if (!skydome->Initialize()) {
		delete skydome;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	skydome->model = model;
	skydome->scale = { 5, 5, 5 };

	return skydome;
}
