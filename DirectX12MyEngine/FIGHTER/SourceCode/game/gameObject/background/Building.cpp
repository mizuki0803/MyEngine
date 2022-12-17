#include "Building.h"

Building* Building::Create(ObjModel* model, const Vector3& position)
{
	//�r���̃C���X�^���X�𐶐�
	Building* building = new Building();
	if (building == nullptr) {
		return nullptr;
	}

	// ������
	if (!building->Initialize()) {
		delete building;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	building->model = model;

	//���W���Z�b�g
	building->position = position;

	//�傫�����Z�b�g
	building->scale = { 5, 5, 5 };

	return building;
}

void Building::FrontOfScreenDelete(const Vector3& pos)
{
	//���W�����@����O(��ʊO��O)�܂ōs������폜
	const float flontOfScreenDiffence = 80;
	const float deletePos = pos.z - flontOfScreenDiffence;

	if (position.z <= deletePos) {
		isDead = true;
	}
}
