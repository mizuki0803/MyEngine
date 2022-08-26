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
	bossMainBody->bornPos = position;

	//�傫�����Z�b�g
	bossMainBody->scale = { 1.5f, 1.5f, 1.5f };

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
	Vector3 stayPos = bornPos;
	stayPos.y = 0;
	position = Easing::Lerp(bornPos, stayPos, time);
}

void BossMainBody::AttackTypeA(const Vector3& playerPosition)
{
	//�v���C���[�����Ɉړ�����
	Vector3 velocity = playerPosition - position;
	const float moveSpeed = 2.0f;
	velocity = velocity.normalize() * moveSpeed;
	position.x += velocity.x;
	position.y += velocity.y;

	//�e���˃^�C�}�[�J�E���g
	fireTimer++;

	//�e���˂��J�n���邩
	if (!isFire) {
		const int firstFireInterval = 200;
		if (fireTimer >= firstFireInterval) { isFire = true; }
		else { return; }
	}

	//���ˊԊu
	const int fireInterval = 10;
	if (fireTimer >= fireInterval) {
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		fireTimer = 0;
	}
}

void BossMainBody::AttackTypeB()
{
	//�^�C�}�[���ړ����鎞�Ԃ��z���Ă����甲����
	const float moveTime = 100;
	if (attackBTimer >= moveTime) { return; }

	//�^�C�}�[���X�V
	attackBTimer++;
	const float time = attackBTimer / moveTime;

	//���Ɉړ�������
	const float moveNum = 30;
	position.z = Easing::OutQuad(bornPos.z, bornPos.z + moveNum, time);
}

void BossMainBody::AttackTypeC(const Vector3& playerPosition)
{
	//�v���C���[�����Ɉړ�����
	Vector3 velocity = playerPosition - position;
	const float moveSpeed = 1.0f;
	velocity = velocity.normalize() * moveSpeed;
	position.x += velocity.x;
	position.y += velocity.y;
}

void BossMainBody::ChangeAttackMode(const float time)
{
	//180�x��]�����Ĕ��Ό����ɂ���
	rotation.y = Easing::InOutBack(waitModeRotY, attackModeRotY, time);
}

void BossMainBody::ChangeWaitMode(const float time)
{
	//180�x��]�����Ĕ��Ό����ɂ���
	rotation.x = 0;
	rotation.y = Easing::InOutBack(attackModeRotY, waitModeRotY, time);
	rotation.z = 0;
}

void BossMainBody::ReturnFixedPosition(const float time)
{
	//���W(0, 0, Z)�ɖ߂�
	position = Easing::Lerp(returnStartPos, { 0, 0, bornPos.z }, time);
}

void BossMainBody::AttackEnd()
{
	//�Ăяo�����u�Ԃ̍��W���Œ�ʒu�ɖ߂�Ƃ��̏o�����W�Ƃ��ċL�^���Ă���
	returnStartPos = position;

	//�e���˃^�C�}�[��������
	fireTimer = 0;
	//�e���ˏ�Ԃ�����
	isFire = false;

	//�U�����eB�̕ϐ��̏�����
	attackBTimer = 0;
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
	const float bulletSpeed = 1.5f;
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, GetWorldPos(), velocity));
	gameScene->AddEnemyBullet(std::move(newBullet));
}
