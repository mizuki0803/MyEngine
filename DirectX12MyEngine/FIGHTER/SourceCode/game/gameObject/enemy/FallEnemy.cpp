#include "FallEnemy.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (FallEnemy::*FallEnemy::actionFuncTable[])() = {
	&FallEnemy::Fall,
	&FallEnemy::Rotate,
	&FallEnemy::Dead,
};


ObjModel* FallEnemy::enemyModel = nullptr;
std::array<ObjModel*, 5> FallEnemy::breakModels;

FallEnemy* FallEnemy::Create(const Vector3& stayPos, const float fallNum)
{
	//�G�̃C���X�^���X�𐶐�
	FallEnemy* upDownEnemy = new FallEnemy();
	if (upDownEnemy == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(enemyModel);
	upDownEnemy->model = FallEnemy::enemyModel;

	// ������
	if (!upDownEnemy->Initialize()) {
		delete upDownEnemy;
		assert(0);
		return nullptr;
	}

	//��~���W���Z�b�g
	upDownEnemy->stayPos = stayPos;

	//�~������l���Z�b�g
	upDownEnemy->fallNum = fallNum;
	
	//���W���Z�b�g
	upDownEnemy->position = stayPos;
	upDownEnemy->position.y += fallNum;

	return upDownEnemy;
}

void FallEnemy::SetBreakModel(int modelNum, ObjModel* model)
{
	//�j�󎞂̃��f�����Z�b�g
	assert(model);
	breakModels[modelNum] = model;
}

void FallEnemy::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//�I�u�W�F�N�g�̍X�V
	Enemy::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();	
}

void FallEnemy::OnCollision()
{
	//�S�G���ʂ̏Փˏ���
	Enemy::OnCollision();

	//�s�������S�p�ɂ���
	phase = Phase::Dead;

	//���S���ė���]���x�𗐐��ŃZ�b�g
	const Vector3 randRotVel = { -5.0f, 0.4f, 0.4f };
	const float randRotBaseVelX = -1.0f;
	crashRotVel.x = (float)rand() / RAND_MAX * randRotVel.x + randRotBaseVelX;
	crashRotVel.y = (float)rand() / RAND_MAX * randRotVel.y - randRotVel.y / 2;
	crashRotVel.z = (float)rand() / RAND_MAX * randRotVel.z - randRotVel.z / 2;
}

void FallEnemy::Fall()
{
	//�C�[�W���O�ō~������
	const float fallTime = 120;
	fallTimer++;
	const float time = fallTimer / fallTime;

	//�����ʒu�~�肽�Ƃ���Œ�~����
	const float startPosY = stayPos.y + fallNum;
	position.y = Easing::OutQuart(startPosY, stayPos.y, time);

	//�C�[�W���O���I�������玟�̃t�F�[�Y��
	if (fallTimer >= fallTime) {
		phase = Phase::Rotate;
	}
}

void FallEnemy::Rotate()
{
	//�N���N����]
	const float rotSpeed = 1.0f;
	rotation.y += rotSpeed;
}

void FallEnemy::Dead()
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

void FallEnemy::Break()
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
