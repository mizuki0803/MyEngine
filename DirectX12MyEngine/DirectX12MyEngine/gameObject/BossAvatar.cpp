#include "BossAvatar.h"
#include "Easing.h"
#include "GameScene.h"

void (BossAvatar::* BossAvatar::attackTypeBPhaseFuncTable[])() = {
	&BossAvatar::AttackTypeBLockon,
	&BossAvatar::AttackTypeBShot,
	&BossAvatar::AttackTypeBBack,
	&BossAvatar::Stay,
};

void (BossAvatar::* BossAvatar::attackTypeCPhaseFuncTable[])() = {
	&BossAvatar::AttackTypeCMoveCenter,
	&BossAvatar::AttackTypeCRotStart,
	&BossAvatar::AttackTypeCShot,
	&BossAvatar::AttackTypeCRotEnd,
	&BossAvatar::Stay,
};

void (BossAvatar::* BossAvatar::attackTypeDPhaseFuncTable[])() = {
	&BossAvatar::AttackTypeDWait,
	&BossAvatar::AttackTypeDMove,
	&BossAvatar::AttackTypeDChargeShot,
	&BossAvatar::AttackTypeDRecoil,
	&BossAvatar::Stay,
};


GameScene* BossAvatar::gameScene = nullptr;
ObjModel* BossAvatar::bulletModel = nullptr;
const float BossAvatar::attackModeRotY = 180.0f;
const float BossAvatar::waitModeRotY = 0.0f;
const float BossAvatar::attackCLength = 1.5f;

bool BossAvatar::Initialize()
{
	//�傫���͖{�̂�3/4
	scale = { 0.75f, 0.75f, 0.75f };

	if (!ObjObject3d::Initialize()) {
		return false;
	}

	return true;
}

void BossAvatar::Damage(int damageNum)
{
	//�����̐����̕��_���[�W����炤
	HP -= damageNum;

	//HP��0�ȉ��ɂȂ����玀�S
	if (HP <= 0) {
		isDead = true;
	}
}

void BossAvatar::AttackTypeB(const Vector3& playerPosition)
{
	//���@���W�����b�N�I������
	attackBLockonPos = playerPosition;

	//�s��
	(this->*attackTypeBPhaseFuncTable[static_cast<size_t>(attackBPhase)])();
}

void BossAvatar::AttackTypeC()
{
	//�s��
	(this->*attackTypeCPhaseFuncTable[static_cast<size_t>(attackCPhase)])();
}

void BossAvatar::AttackTypeD()
{
	//�s��
	(this->*attackTypeDPhaseFuncTable[static_cast<size_t>(attackDPhase)])();
}

void BossAvatar::ChangeAttackMode(const float time)
{
	//180�x��]�����Ĕ��Ό����ɂ���
	rotation.y = Easing::InOutBack(waitModeRotY, attackModeRotY, time);
}

void BossAvatar::ChangeWaitMode(const float time)
{
	//180�x��]�����Ĕ��Ό����ɂ���
	rotation.x = 0;
	rotation.y = Easing::InOutBack(attackModeRotY, waitModeRotY, time);
	rotation.z = 0;
}

void BossAvatar::ReturnBasePosition(const float time)
{
	//�e�q�֌W��ł̊�ʒu���W�ɖ߂�
	position.x = Easing::OutQuad(returnStartPos.x, basePos.x, time);
	position.y = Easing::OutQuad(returnStartPos.y, basePos.y, time);
	position.z = Easing::OutQuad(returnStartPos.z, basePos.z, time);
}

void BossAvatar::AttackEnd()
{
	//�Ăяo�����u�Ԃ̍��W����ʒu�ɖ߂�Ƃ��̏o�����W�Ƃ��ċL�^���Ă���
	returnStartPos = position;

	//�e���˃^�C�}�[��������
	fireTimer = 0;

	//�U�����eB�̕ϐ��̏�����
	attackBPhase = AttackTypeBPhase::Lockon;
	attackBTimer = 0;

	//�U�����eC�̕ϐ��̏�����
	attackCPhase = AttackTypeCPhase::MoveCenter;
	attackCTimer = 0;
	attackCRotSpeed = 0;

	//�U�����eD�̕ϐ��̏�����
	attackDPhase = AttackTypeDPhase::Wait;
	attackDTimer = 0;
	attackDRecoilVelocity = { 0, 0, 2 };
	attackDRecoilAccel = { 0, 0, -0.05f };
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

void BossAvatar::Fire(const float scale, const float bulletSpeed)
{
	//�e�̑��x��ݒ�
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, GetWorldPos(), velocity, scale));
	gameScene->AddEnemyBullet(std::move(newBullet));
}

void BossAvatar::AttackTypeBShot()
{
	//���b�N�I���Ō��߂����x�Ŕ�΂�
	position += attackBVelocity;

	//�^�C�}�[���X�V
	const float shotTime = 180;
	attackBTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackBTimer >= shotTime) {
		attackBPhase = AttackTypeBPhase::Back;

		//�^�C�}�[������
		attackBTimer = 0;
	}
}

void BossAvatar::AttackTypeBBack()
{
	//�^�C�}�[���X�V
	const float backTime = 120;
	attackBTimer++;
	const float time = attackBTimer / backTime;

	//�e�q�֌W��ł̊���W�ɖ߂�
	position.x = Easing::OutQuad(0, basePos.x, time);
	position.y = Easing::OutQuad(60, basePos.y, time);
	position.z = Easing::OutQuad(0, basePos.z, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackBTimer >= backTime) {
		attackBPhase = AttackTypeBPhase::Stay;

		//�^�C�}�[������
		attackBTimer = 0;
	}
}

void BossAvatar::AttackTypeCRotStart()
{
	//��]���J�n(��]���x�𑬂�����)
	const float rotAccelSpeed = 0.1f;
	attackCRotSpeed += rotAccelSpeed;

	//�p�x���ʓx�@�ɕϊ�
	float radius = XMConvertToRadians(attackCAngle);

	//�~�̒��S����w�肵�����a�̒��������邮����
	position.x = cos(radius) * attackCLength;
	position.y = sin(radius) * attackCLength;

	//�p�x�̍X�V
	attackCAngle += attackCRotSpeed;

	//��]���x���w�肵�����x�ȏ�ɂȂ����玟�̃t�F�[�Y��
	const float maxRotSpeed = 4.0f;
	if (attackCRotSpeed >= maxRotSpeed) {
		attackCPhase = AttackTypeCPhase::Shot;
	}
}

void BossAvatar::AttackTypeCShot()
{
	//�^�C�}�[���X�V
	const float moveTime = 300;
	attackCTimer++;

	//�p�x���ʓx�@�ɕϊ�
	float radius = XMConvertToRadians(attackCAngle);

	//�~�̒��S����w�肵�����a�̒��������邮����
	position.x = cos(radius) * attackCLength;
	position.y = sin(radius) * attackCLength;

	//�p�x�̍X�V
	attackCAngle += attackCRotSpeed;

	//�e���˃^�C�}�[�J�E���g
	fireTimer++;
	const int fireInterval = 10;
	if (fireTimer >= fireInterval) {
		//�e�𔭎�
		const float bulletScale = 1.0f;
		const float bulletSpeed = 1.0f;
		Fire(bulletScale, bulletSpeed);
		//���˃^�C�}�[��������
		fireTimer = 0;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackCTimer >= moveTime) {
		attackCPhase = AttackTypeCPhase::RotEnd;

		//�^�C�}�[������
		attackCTimer = 0;
	}
}

void BossAvatar::AttackTypeCRotEnd()
{
	//��]���I��(��]���x��x������)
	const float rotAccelSpeed = -0.1f;
	attackCRotSpeed += rotAccelSpeed;

	//�p�x���ʓx�@�ɕϊ�
	float radius = XMConvertToRadians(attackCAngle);

	//�~�̒��S����w�肵�����a�̒��������邮����
	position.x = cos(radius) * attackCLength;
	position.y = sin(radius) * attackCLength;

	//�p�x�̍X�V
	attackCAngle += attackCRotSpeed;

	//��]���x��0�ȉ��ɂȂ����玟�̃t�F�[�Y��
	if (attackCRotSpeed <= 0) {
		attackCPhase = AttackTypeCPhase::Stay;
	}
}

void BossAvatar::AttackTypeDChargeShot()
{
	//�^�C�}�[���X�V
	const float chargeTime = 180;
	attackDTimer++;
	const float time = attackDTimer / chargeTime;

	//�F��Ԃ�����
	color.y = Easing::OutQuad(1, 0, time);
	color.z = Easing::OutQuad(1, 0, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackDTimer >= chargeTime) {
		attackDPhase = AttackTypeDPhase::Recoil;

		//�^�C�}�[������
		attackDTimer = 0;

		//�e�𔭎�
		const float bulletScale = 10.0f;
		const float bulletSpeed = 0.8f;
		Fire(bulletScale, bulletSpeed);

		//�F�����ɖ߂�
		color = { 1, 1, 1, 1 };
	}
}

void BossAvatar::AttackTypeDRecoil()
{
	//�����ŉ��Ɉړ�������
	position += attackDRecoilVelocity;
	attackDRecoilVelocity += attackDRecoilAccel;

	//�����̑��x��0�ɂȂ����玟�̃t�F�[�Y��
	if (attackDRecoilVelocity.z <= 0) {
		attackDPhase = AttackTypeDPhase::Stay;
	}
}

void BossAvatar::Stay()
{
}
