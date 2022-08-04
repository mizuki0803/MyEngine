#include "CircularEnemy.h"

CircularEnemy* CircularEnemy::Create(ObjModel* model, const Vector3& centerPosition, const float angle, const float length, const float rotSpeed)
{
	//�G�̃C���X�^���X�𐶐�
	CircularEnemy* circularEnemy = new CircularEnemy();
	if (circularEnemy == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	circularEnemy->model = model;

	// ������
	if (!circularEnemy->Initialize()) {
		delete circularEnemy;
		assert(0);
		return nullptr;
	}

	//���S���W���Z�b�g
	circularEnemy->centerPosition = centerPosition;

	//�����p�x���Z�b�g
	circularEnemy->angle = angle;

	//���a�̒������Z�b�g
	circularEnemy->length = length;

	//��]���x���Z�b�g
	circularEnemy->rotSpeed = rotSpeed;

	return circularEnemy;
}

void CircularEnemy::Update()
{
	//���˃^�C�}�[�J�E���g�_�E��
	--fireTimer;
	if (fireTimer <= 0) {
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		fireTimer = fireInterval;
	}

	//�ړ�
	Move();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();
}

void CircularEnemy::Move()
{
	//�p�x���ʓx�@�ɕϊ�
	float radius = XMConvertToRadians(angle);

	//�~�̒��S����w�肵�����a�̒��������邮����
	position.x = cos(radius) * length + centerPosition.x;
	position.y = sin(radius) * length + centerPosition.y;
	position.z = centerPosition.z;

	//�p�x�̍X�V
	angle += rotSpeed;
}
