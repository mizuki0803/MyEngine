#include "BossAvatar.h"
#include "Easing.h"
#include "GameScene.h"

GameScene* BossAvatar::gameScene = nullptr;
ObjModel* BossAvatar::bulletModel = nullptr;
const float BossAvatar::attackModeRotY = 180.0f;
const float BossAvatar::waitModeRotY = 0.0f;

void BossAvatar::Damage(int damageNum)
{
	//�����̐����̕��_���[�W����炤
	HP -= damageNum;

	//HP��0�ȉ��ɂȂ����玀�S
	if (HP <= 0) {
		isDead = true;
	}
}

void BossAvatar::AttackTypeB()
{
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

void BossAvatar::ChangeAttackMode(const float time)
{
	//180�x��]�����Ĕ��Ό����ɂ���
	rotation.y = Easing::InOutBack(waitModeRotY, attackModeRotY, time);
}

void BossAvatar::ChangeWaitMode(const float time)
{
	//180�x��]�����Ĕ��Ό����ɂ���
	rotation.y = Easing::InOutBack(attackModeRotY, waitModeRotY, time);
}

void BossAvatar::ReturnFixedPosition(const float time)
{
	//�e�q�֌W��ł̒�ʒu���W�ɖ߂�
	position.x = Easing::OutQuad(returnStartPos.x, fixedPos.x, time);
	position.y = Easing::OutQuad(returnStartPos.y, fixedPos.y, time);
	position.z = Easing::OutQuad(returnStartPos.z, fixedPos.z, time);
}

void BossAvatar::SetReturnStartPos()
{
	//�Ăяo�����u�Ԃ̍��W���Œ�ʒu�ɖ߂�Ƃ��̏o�����W�Ƃ��ċL�^���Ă���
	returnStartPos = position;
}

Vector3 BossAvatar::GetWorldPos()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���s�ړ��������擾
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void BossAvatar::Fire()
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
