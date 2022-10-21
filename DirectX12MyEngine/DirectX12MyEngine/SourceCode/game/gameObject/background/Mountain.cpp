#include "Mountain.h"

Mountain* Mountain::Create(ObjModel* model, const Vector3& position)
{
	//�R�̃C���X�^���X�𐶐�
	Mountain* mountain = new Mountain();
	if (mountain == nullptr) {
		return nullptr;
	}

	// ������
	if (!mountain->Initialize()) {
		delete mountain;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	mountain->model = model;

	//���W���Z�b�g
	mountain->position = position;

	//�傫�����Z�b�g
	mountain->scale = { 10, 10, 10 };

	return mountain;
}
