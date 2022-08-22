#include "BossMainBody.h"
#include "Easing.h"
#include "GameScene.h"

GameScene* BossMainBody::gameScene = nullptr;
ObjModel* BossMainBody::bulletModel = nullptr;
const float BossMainBody::attackModeRotY = 180.0f;
const float BossMainBody::waitModeRotY = 0.0f;

BossMainBody* BossMainBody::Create(ObjModel* model, const Vector3& position)
{
	//�{�X(�{��)�̃C���X�^���X�𐶐�
	BossMainBody* bossMainBody = new BossMainBody();
	if (bossMainBody == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	bossMainBody->model = model;

	// ������
	if (!bossMainBody->Initialize()) {
		delete bossMainBody;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	bossMainBody->position = position;

	//�������W���Z�b�g
	bossMainBody->startPos = position;

	return bossMainBody;
}

void BossMainBody::Damage(int damageNum)
{
	//�����̐����̕��_���[�W����炤
	HP -= damageNum;

	//HP��0�ȉ��ɂȂ����玀�S
	if (HP <= 0) {
		isDead = true;
	}
}

void BossMainBody::Fall(const float time)
{
	//�����ʒu����~�肽�Ƃ���Œ�~����
	Vector3 stayPos = startPos;
	stayPos.y = 0;
	position = Easing::Lerp(startPos, stayPos, time);
}

void BossMainBody::AttackTypeA()
{
	position.x += 0.01f;

	//�e���˃^�C�}�[�J�E���g
	fireTimer++;
	const int fireInterval = 30;
	if (fireTimer >= fireInterval) {
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		fireTimer = 0;
	}
}

void BossMainBody::AttackTypeB()
{
	position.x -= 0.01f;
}

void BossMainBody::ChangeAttackMode(const float time)
{
	//180�x��]�����Ĕ��Ό����ɂ���
	rotation.y = Easing::InOutBack(waitModeRotY, attackModeRotY, time);
}

void BossMainBody::ChangeWaitMode(const float time)
{
	//180�x��]�����Ĕ��Ό����ɂ���
	rotation.y = Easing::InOutBack(attackModeRotY, waitModeRotY, time);
}

Vector3 BossMainBody::GetWorldPos()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���s�ړ��������擾
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void BossMainBody::Fire()
{
	//�e�̑��x��ݒ�
	const float bulletSpeed = 0.5f;
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, GetWorldPos(), velocity));
	gameScene->AddEnemyBullet(std::move(newBullet));
}
