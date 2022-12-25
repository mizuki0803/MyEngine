#include "Meteorite.h"

ObjModel* Meteorite::meteoriteModel = nullptr;
GameCamera* Meteorite::gameCamera = nullptr;

Meteorite* Meteorite::Create(const Vector3& position, const Vector3& rotation, float size, const Vector3& velocity, const Vector3& rotSpeed)
{
	//覐΂̃C���X�^���X�𐶐�
	Meteorite* meteorite = new Meteorite();
	if (meteorite == nullptr) {
		return nullptr;
	}

	// ������
	if (!meteorite->Initialize()) {
		delete meteorite;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(meteoriteModel);
	meteorite->model = meteoriteModel;
	//���W���Z�b�g
	meteorite->position = position;
	//��]���x���Z�b�g
	meteorite->rotSpeed = rotSpeed;
	//�傫�����Z�b�g
	meteorite->scale = { size, size, size };
	//�ړ����x���Z�b�g
	meteorite->velocity = velocity;
	//��]�p���Z�b�g
	meteorite->rotation = rotation;

	return meteorite;
}

void Meteorite::Update()
{
	//���W���X�V
	position += velocity;
	//��]�p���X�V
	rotation += rotSpeed;

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();

	//��O�܂ōs������폜
	FrontOfScreenDelete();
}

void Meteorite::FrontOfScreenDelete()
{
	//���W�����@����O(��ʊO��O)�܂ōs������폜
	const float flontOfScreenDiffence = 50;
	const float deletePos = gameCamera->GetPosition().z - flontOfScreenDiffence;

	if (position.z <= deletePos) {
		isDelete = true;
	}
}
