#include "EnemyBreakEffect.h"

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
    //�ړ�
    Move();

    //�I�u�W�F�N�g�X�V
    ObjObject3d::Update();
}

void EnemyBreakEffect::Move()
{
    //�d�͂��ۂ��l��Y���ɂ���������
    const float gravity = 0.01f;
    velocity.y -= gravity;
    position += velocity;

	//��]������
	rotation += rotSpeed;

    //�n�ʂɒ�������폜��Ԃɂ���
    if (position.y <= 0.0f) {
        isDelete = true;
    }
}
