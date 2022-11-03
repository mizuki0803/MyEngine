#include "StageSelectPlanet.h"

StageSelectPlanet* StageSelectPlanet::Create(ObjModel* model, const Vector3& position)
{
	//�X�e�[�W�I��p�f���̃C���X�^���X�𐶐�
	StageSelectPlanet* stageSelectPlanet = new StageSelectPlanet();
	if (stageSelectPlanet == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	stageSelectPlanet->model = model;

	// ������
	if (!stageSelectPlanet->Initialize()) {
		delete stageSelectPlanet;
		assert(0);
		return nullptr;
	}

	//�������W���Z�b�g
	stageSelectPlanet->position = position;

	//�傫�����Z�b�g
	const float size = 25.0f;
	stageSelectPlanet->scale = { size, size, size };

	return stageSelectPlanet;
}

void StageSelectPlanet::Update()
{
	//��]������
	const float rotSpeed = 0.1f;
	rotation.y += rotSpeed;

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}
