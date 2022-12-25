#include "MeteoriteEnemy.h"
#include "ParticleEmitter.h"

ObjModel* MeteoriteEnemy::meteoriteModel = nullptr;

MeteoriteEnemy* MeteoriteEnemy::Create(const Vector3& position, const Vector3& rotation, float size, const Vector3& velocity, const Vector3& rotSpeed, int HP)
{
	//覐΂̃C���X�^���X�𐶐�
	MeteoriteEnemy* meteoriteEnemy = new MeteoriteEnemy();
	if (meteoriteEnemy == nullptr) {
		return nullptr;
	}

	// ������
	if (!meteoriteEnemy->Initialize()) {
		delete meteoriteEnemy;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(meteoriteModel);
	meteoriteEnemy->model = meteoriteModel;
	//���W���Z�b�g
	meteoriteEnemy->position = position;
	//��]�p���Z�b�g
	meteoriteEnemy->rotation = rotation;
	//�傫�����Z�b�g
	meteoriteEnemy->scale = { size, size, size };
	//�ʏ�T�C�Y���Z�b�g
	meteoriteEnemy->normalSize = meteoriteEnemy->scale;
	//�_���[�W��Ԃ̃T�C�Y���Z�b�g
	meteoriteEnemy->damageSize = meteoriteEnemy->scale * 1.1f;
	//�ړ����x���Z�b�g
	meteoriteEnemy->velocity = velocity;
	//��]���x���Z�b�g
	meteoriteEnemy->rotSpeed = rotSpeed;
	//HP���Z�b�g
	meteoriteEnemy->HP = HP;

	return meteoriteEnemy;
}

void MeteoriteEnemy::Update()
{
	//���W���X�V
	position += velocity;
	//��]�p���X�V
	rotation += rotSpeed;

	//���S���
	if (isDead) {
		Dead();
	}

	//�I�u�W�F�N�g�X�V
	Enemy::Update();

	//��O�܂ōs������폜
	FrontOfScreenDelete();
}

void MeteoriteEnemy::Dead()
{
	//�폜�܂łɂ����鎞��
	const int deadModeTime = 2;
	//�^�C�}�[���X�V
	deadTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (deadTimer >= deadModeTime) {
		//�폜����
		isDelete = true;

		//�j��G�t�F�N�g���o��
		Break();

		//�����p�傫��
		float explosionSize = 4;
		//��������
		int explosionTime = 30;
		//�������o�p�p�[�e�B�N������
		ParticleEmitter::GetInstance()->Explosion(position, explosionSize, explosionTime);
	}
}

void MeteoriteEnemy::Break()
{
	//�j��p�G�t�F�N�g�ǉ�
	for (int i = 0; i < 10; i++) {
		//���f�������ݒ�Ȃ��΂�
		if (!meteoriteModel) { continue; }

		//�����_���ŃG�t�F�N�g�̑��x���Z�b�g
		const Vector3 randVel = { 6, 6, 6 };
		Vector3 velocity;
		velocity.x = (float)((rand() % (int)randVel.x) - randVel.x / 2);
		velocity.y = (float)((rand() % (int)randVel.y) - randVel.y / 2);
		velocity.z = (float)((rand() % (int)randVel.z) - randVel.z / 2);

		//�����_���ŃG�t�F�N�g�̉�]�̑������Z�b�g
		const Vector3 randRotSpeed = { 5, 5, 5 };
		Vector3 rotSpeed;
		rotSpeed.x = (float)((rand() % (int)randRotSpeed.x) - randRotSpeed.x / 2);
		rotSpeed.y = (float)((rand() % (int)randRotSpeed.y) - randRotSpeed.y / 2);
		rotSpeed.z = (float)((rand() % (int)randRotSpeed.z) - randRotSpeed.z / 2);

		//�l���傫���̂Ŋ���Z���ď���������
		const float div = 4;
		velocity /= div;
		//�傫�����Z�b�g
		const Vector3 scale = this->scale / 5;

		//�j�󎞃G�t�F�N�g����
		BreakEffect(meteoriteModel, velocity, rotSpeed, scale);
	}
}
