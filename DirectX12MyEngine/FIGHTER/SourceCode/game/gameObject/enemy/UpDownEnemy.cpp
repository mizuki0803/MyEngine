#include "UpDownEnemy.h"
#include "Player.h"
#include "ParticleEmitter.h"

void (UpDownEnemy::* UpDownEnemy::actionFuncTable[])() = {
	&UpDownEnemy::UpBrake,
	&UpDownEnemy::DownBrake,
	&UpDownEnemy::Dead,
};

ObjModel* UpDownEnemy::enemyModel = nullptr;
std::array<ObjModel*, 5> UpDownEnemy::breakModels;

UpDownEnemy* UpDownEnemy::Create(const Vector3& position)
{
	//�G�̃C���X�^���X�𐶐�
	UpDownEnemy* upDownEnemy = new UpDownEnemy();
	if (upDownEnemy == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(enemyModel);
	upDownEnemy->model = UpDownEnemy::enemyModel;

	// ������
	if (!upDownEnemy->Initialize()) {
		delete upDownEnemy;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	upDownEnemy->position = position;

	return upDownEnemy;
}

void UpDownEnemy::SetBreakModel(int modelNum, ObjModel* model)
{
	//�j�󎞂̃��f�����Z�b�g
	assert(model);
	breakModels[modelNum] = model;
}

void UpDownEnemy::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//�I�u�W�F�N�g�̍X�V
	Enemy::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();
}

void UpDownEnemy::OnCollision()
{
	//�S�G���ʂ̏Փˏ���
	Enemy::OnCollision();

	//�s�������S�p�ɂ���
	phase = Phase::Dead;

	//���S���ė���]���x�𗐐��ŃZ�b�g
	const float randRotVelX = -1.0f;
	const float randRotBaseVelX = -0.5f;
	crashRotVel.x = (float)rand() / RAND_MAX * randRotVelX + randRotBaseVelX;
}

void UpDownEnemy::UpBrake()
{
	//�㏸��}����
	Vector3 accel = { 0, -0.01f, 0 };
	velocity += accel;
	position += velocity;


	//�w�肵�����x�܂ŉ��~��ԂɂȂ����牺�~�Ƀu���[�L��������
	const float changePhaseVelY = -0.5f;
	if (velocity.y <= changePhaseVelY) {
		phase = Phase::DownBrake;
	}
}

void UpDownEnemy::DownBrake()
{
	//���~��}����
	Vector3 accel = { 0, 0.01f, 0 };
	velocity += accel;
	position += velocity;


	//�w�肵�����x�܂ŏ㏸��ԂɂȂ�����㏸�Ƀu���[�L��������
	const float changePhaseVelY = 0.5f;
	if (velocity.y >= changePhaseVelY) {
		phase = Phase::UpBrake;
	}
}

void UpDownEnemy::Dead()
{
	//��]�����Ȃ���ė�����
	rotation += crashRotVel;

	//�ė����邽�߁A���x�ɉ����x��������
	Vector3 crashAccel = { 0, -0.01f, 0 };
	crashVel += crashAccel;
	//�������鑬�x�̍ő�l��ݒ�
	const float maxCrashSpeed = -0.5f;
	if (crashVel.y <= maxCrashSpeed) { crashVel.y = maxCrashSpeed; }
	position += crashVel;

	//Y���W��0�ȉ��ɂȂ�����폜
	if (position.y <= 0) {
		isDelete = true;

		//�������o�p�p�[�e�B�N������
		ParticleEmitter::GetInstance()->Explosion(position);
	}
}

void UpDownEnemy::Break()
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
