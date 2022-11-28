#include "CircularEnemy.h"

ObjModel* CircularEnemy::enemyModel = nullptr;
std::array<ObjModel*, 5> CircularEnemy::breakModels;

CircularEnemy* CircularEnemy::Create(const Vector3& centerPosition, const float angle, const float length, const float rotSpeed)
{
	//�G�̃C���X�^���X�𐶐�
	CircularEnemy* circularEnemy = new CircularEnemy();
	if (circularEnemy == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(enemyModel);
	circularEnemy->model = CircularEnemy::enemyModel;

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

void CircularEnemy::SetBreakModel(int modelNum, ObjModel* model)
{
	//�j�󎞂̃��f�����Z�b�g
	assert(model);
	breakModels[modelNum] = model;
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

	//�I�u�W�F�N�g�̍X�V
	Enemy::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();
}

void CircularEnemy::OnCollision()
{
	//�S�G���ʂ̏Փˏ���
	Enemy::OnCollision();

	//�폜����
	isDelete = true;

	//�j��
	Break();
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

void CircularEnemy::Break()
{
	//�j��p�G�t�F�N�g�ǉ�
	for (int i = 0; i < breakModels.size(); i++) {
		//���f�������ݒ�Ȃ��΂�
		if (!breakModels[i]) { continue; }

		//�����_���ŃG�t�F�N�g�̑��x���Z�b�g
		const Vector3 randVel = { 4, 4, 4 };
		Vector3 velocity;
		velocity.x = (float)((rand() % (int)randVel.x) - randVel.x / 2);
		velocity.y = (float)((rand() % (int)randVel.y) - randVel.y / 4);
		velocity.z = (float)((rand() % (int)randVel.z) - randVel.z / 2);

		//�����_���ŃG�t�F�N�g�̉�]�̑������Z�b�g
		const Vector3 randRotSpeed = { 5, 5, 5 };
		Vector3 rotSpeed;
		rotSpeed.x = (float)((rand() % (int)randRotSpeed.x) - randRotSpeed.x / 2);
		rotSpeed.x = (float)((rand() % (int)randRotSpeed.y) - randRotSpeed.y / 2);
		rotSpeed.z = (float)((rand() % (int)randRotSpeed.z) - randRotSpeed.z / 2);

		//�l���傫���̂Ŋ���Z���ď���������
		const float div = 10;
		velocity /= div;
		//�傫�����Z�b�g
		const Vector3 scale = { 1.5f, 1.5f, 1.5f };

		//�j�󎞃G�t�F�N�g����
		BreakEffect(breakModels[i], velocity, rotSpeed, scale);
	}
}
