#include "CannonEnemy.h"
#include "ParticleEmitter.h"

void (CannonEnemy::* CannonEnemy::actionFuncTable[])() = {
	&CannonEnemy::Attack,
	&CannonEnemy::Dead,
};

ObjModel* CannonEnemy::enemyModel = nullptr;
std::array<ObjModel*, 5> CannonEnemy::breakModels;

CannonEnemy* CannonEnemy::Create(const Vector3& position)
{
	//�G�̃C���X�^���X�𐶐�
	CannonEnemy* cannon = new CannonEnemy();
	if (cannon == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(enemyModel);
	cannon->model = CannonEnemy::enemyModel;

	// ������
	if (!cannon->Initialize()) {
		delete cannon;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	cannon->position = position;

	//�傫�����Z�b�g
	const float scaleNum = 3;
	cannon->scale = { scaleNum, scaleNum, scaleNum };
	//�ʏ�T�C�Y���Z�b�g
	cannon->normalSize = cannon->scale;
	//�_���[�W��Ԃ̃T�C�Y���Z�b�g
	cannon->damageSize = cannon->scale * 1.1f;
	//HP���Z�b�g
	cannon->HP = maxHP;

	return cannon;
}

void CannonEnemy::SetBreakModel(int modelNum, ObjModel* model)
{
	//�j�󎞂̃��f�����Z�b�g
	assert(model);
	breakModels[modelNum] = model;
}

void CannonEnemy::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//�I�u�W�F�N�g�̍X�V
	Enemy::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();
}

void CannonEnemy::OnCollision(const int damageNum)
{
	//�S�G���ʂ̏Փˏ���
	Enemy::OnCollision(damageNum);

	//���S������
	if (isDead) {
		//�s�������S�p�ɂ���
		phase = Phase::Dead;

		//���S���ė���]���x�𗐐��ŃZ�b�g
		const Vector3 randRotVel = { -2.0f, 0.4f, 0.4f };
		const float randRotBaseVelX = -1.75f;
		crashRotVel.x = (float)rand() / RAND_MAX * randRotVel.x + randRotBaseVelX;
		crashRotVel.y = (float)rand() / RAND_MAX * randRotVel.y - randRotVel.y / 2;
		crashRotVel.z = (float)rand() / RAND_MAX * randRotVel.z - randRotVel.z / 2;
	}
}

void CannonEnemy::Attack()
{
	//���˃^�C�}�[�J�E���g�_�E��
	--fireTimer;
	if (fireTimer <= 0) {
		//�e�𔭎�
		FirePlayerAngle();
		//���˃^�C�}�[��������
		fireTimer = fireInterval;
	}
}

void CannonEnemy::Dead()
{
	//��]�����Ȃ���ė�����
	rotation += crashRotVel;

	//�ė����邽�߁A���x�ɉ����x��������
	Vector3 crashAccel = { 0, -0.005f, 0.001f };
	crashVel += crashAccel;
	//�������鑬�x�̍ő�l��ݒ�
	const float maxCrashSpeed = -0.2f;
	if (crashVel.y <= maxCrashSpeed) { crashVel.y = maxCrashSpeed; }
	position += crashVel;

	//�����p�[�e�B�N������
	const float smokeSize = scale.x * 2.0f;
	DeadSmokeEffect(smokeSize);

	//�n�ʂ���̂Ƃ�
	if (isGroundMode) {
		//Y���W��0�ȉ��ɂȂ�����폜
		if (position.y <= 0) {
			isDelete = true;

			//�������o�p�p�[�e�B�N������
			ParticleEmitter::GetInstance()->Explosion(position);
		}
	}
	//�n�ʂȂ��̂Ƃ�
	else {
		//Y���W���w�肵���l�ȉ��ɂȂ�����폜
		const float deletePos = -50;
		if (position.y <= deletePos) {
			isDelete = true;
		}
	}
}

void CannonEnemy::Break()
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
		rotSpeed.y = (float)((rand() % (int)randRotSpeed.y) - randRotSpeed.y / 2);
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
