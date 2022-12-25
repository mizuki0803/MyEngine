#include "EnemyBreakEffect.h"

bool EnemyBreakEffect::isGroundMode = true;
bool EnemyBreakEffect::isGravityMode = false;
GameCamera* EnemyBreakEffect::gameCamera = nullptr;

EnemyBreakEffect* EnemyBreakEffect::Create(ObjModel* model, const Vector3& position, const Vector3& velocity, const Vector3& rotSpeed, const Vector3& scale)
{
	//��ʃG�t�F�N�g�̃C���X�^���X�𐶐�
	EnemyBreakEffect* enemyBreakEffect = new EnemyBreakEffect();
	if (enemyBreakEffect == nullptr) {
		return nullptr;
	}

	// ������
	if (!enemyBreakEffect->Initialize()) {
		delete enemyBreakEffect;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	enemyBreakEffect->model = model;

	//���W���Z�b�g
	enemyBreakEffect->position = position;

	//���x���Z�b�g
	enemyBreakEffect->velocity = velocity;

	//��]�̑������Z�b�g
	enemyBreakEffect->rotSpeed = rotSpeed;

	//�傫�����Z�b�g
	enemyBreakEffect->scale = scale;

	return enemyBreakEffect;
}

void EnemyBreakEffect::Update()
{
	//���Ԍo�߂ō폜
	if (--deathTimer <= 0) {
		isDelete = true;
	}

    //�ړ�
    Move();

    //�I�u�W�F�N�g�X�V
    ObjObject3d::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();
}

void EnemyBreakEffect::Move()
{
	//�d�͂����ԂȂ�
	if (isGravityMode) {
		//�d�͂��ۂ��l��Y���ɂ���������
		const float gravity = 0.01f;
		velocity.y -= gravity;
	}
	//���x�����W�ɉ��Z���Ĉړ�������
    position += velocity;

	//��]������
	rotation += rotSpeed;

    //�n�ʂ����Ԃ̏ꍇ�̂ݒn�ʂɒ�������폜��Ԃɂ���
    if (isGroundMode && position.y <= 0.0f) {
        isDelete = true;
    }
}

void EnemyBreakEffect::FrontOfScreenDelete()
{
	//���W�����@����O(��ʊO��O)�܂ōs������폜
	const float flontOfScreenDiffence = 50;
	const float deletePos = gameCamera->GetPosition().z - flontOfScreenDiffence;

	if (position.z <= deletePos) {
		isDelete = true;
	}
}
