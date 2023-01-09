#include "GalaxyPropeller.h"

ObjModel* GalaxyPropeller::propellerModel = nullptr;

GalaxyPropeller* GalaxyPropeller::Create(ObjObject3d* parent, const Vector3& position, const Vector3& rotation, const float rotSpeed, const float size)
{
	//�M�����N�V�[(�v���y��)�̃C���X�^���X�𐶐�
	GalaxyPropeller* galaxyPropeller = new GalaxyPropeller();
	if (galaxyPropeller == nullptr) {
		return nullptr;
	}

	// ������
	if (!galaxyPropeller->Initialize(parent, position, rotation, rotSpeed, size)) {
		delete galaxyPropeller;
		assert(0);
		return nullptr;
	}

	return galaxyPropeller;
}

bool GalaxyPropeller::Initialize(ObjObject3d* parent, const Vector3& position, const Vector3& rotation, const float rotSpeed, const float size)
{
	//�I�u�W�F�N�g������
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//���f�����Z�b�g
	assert(propellerModel);
	model = propellerModel;

	//�e�I�u�W�F�N�g���Z�b�g
	this->parent = parent;
	//���W���Z�b�g
	this->position = position;
	//��]�p���Z�b�g
	this->rotation = rotation;
	//��]���x���Z�b�g
	this->rotSpeed = rotSpeed;
	//�傫�����Z�b�g
	scale = { size, size, size };

	return true;
}

void GalaxyPropeller::Update()
{
	//��]
	if (isRotation) {
		Rotation();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();

	Vector3 a = GetWorldPos();
}

void GalaxyPropeller::Rotation()
{
	//��]���x�����Z���ĉ�]������
	rotation.y += rotSpeed;

	//�p�x�C��
	if (rotation.y <= 0) { rotation.y += 360; }
	if (rotation.y >= 360) { rotation.y -= 360; }
}
