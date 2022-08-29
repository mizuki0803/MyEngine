#include "BossMainBody.h"
#include "Easing.h"
#include "GameScene.h"

void (BossMainBody::* BossMainBody::attackTypeAPhaseFuncTable[])() = {
	&BossMainBody::AttackTypeALockon,
	&BossMainBody::AttackTypeAShot,
	&BossMainBody::Stay,
};

void (BossMainBody::* BossMainBody::attackTypeA2PhaseFuncTable[])() = {
	&BossMainBody::AttackTypeA2Move,
	&BossMainBody::AttackTypeA2ChargeShot,
	&BossMainBody::Stay,
};

void (BossMainBody::* BossMainBody::attackTypeA3PhaseFuncTable[])() = {
	&BossMainBody::AttackTypeA3Wait,
	&BossMainBody::AttackTypeA3Shot,
	&BossMainBody::AttackTypeA3Move,
	&BossMainBody::Stay,
};

GameScene* BossMainBody::gameScene = nullptr;
ObjModel* BossMainBody::bulletModel = nullptr;
const float BossMainBody::attackModeRotY = 180.0f;
const float BossMainBody::waitModeRotY = 0.0f;

BossMainBody* BossMainBody::Create(ObjModel* model, const Vector3& basePos)
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

	//��~�����̍��W���Z�b�g
	bossMainBody->basePos = basePos;

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
	//��̈ʒu�̐^�ォ��~��Ă���
	Vector3 bornPos = basePos;
	const float fallNum = 70;
	bornPos.y = basePos.y + fallNum;
	position = Easing::Lerp(bornPos, basePos, time);
}

void BossMainBody::AttackTypeA(const Vector3& playerPosition)
{
	//��Ƀv���C���[�����Ɉړ�����
	Vector3 velocity = playerPosition - position;
	const float moveSpeed = 2.0f;
	velocity = velocity.normalize() * moveSpeed;
	position.x += velocity.x;
	position.y += velocity.y;

	//�s��
	(this->*attackTypeAPhaseFuncTable[static_cast<size_t>(attackAPhase)])();
}

void BossMainBody::AttackTypeA2()
{
	//�s��
	(this->*attackTypeA2PhaseFuncTable[static_cast<size_t>(attackA2Phase)])();
}

void BossMainBody::AttackTypeA3()
{
	//�s��
	(this->*attackTypeA3PhaseFuncTable[static_cast<size_t>(attackA3Phase)])();
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
	position.z = Easing::OutQuad(basePos.z, basePos.z + moveNum, time);
}

void BossMainBody::AttackTypeC(const Vector3& playerPosition)
{
	//��Ƀv���C���[�����Ɉړ�����
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

void BossMainBody::ReturnBasePosition(const float time)
{
	//��̍��W�ɖ߂�
	position = Easing::Lerp(returnStartPos, basePos, time);
}

void BossMainBody::AttackEnd()
{
	//�Ăяo�����u�Ԃ̍��W����ʒu�ɖ߂�Ƃ��̏o�����W�Ƃ��ċL�^���Ă���
	returnStartPos = position;

	//�e���˃^�C�}�[��������
	fireTimer = 0;

	//�U�����eA�̕ϐ��̏�����
	attackAPhase = AttackTypeAPhase::Lockon;
	attackATimer = 0;

	//�U�����eA2�̕ϐ��̏�����
	attackA2Phase = AttackTypeA2Phase::Move;
	attackA2Timer = 0;

	//�U�����eA3�̕ϐ��̏�����
	attackA3Phase = AttackTypeA3Phase::Wait;
	attackA3Timer = 0;
	attackA3ShotCount = 0;
	attackA3MoveBeforePos = {};
	attackA3MpveAfterPos = {};

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

void BossMainBody::Fire(const float scale, const float bulletSpeed)
{
	//�e�̑��x��ݒ�
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, GetWorldPos(), velocity, scale));
	gameScene->AddEnemyBullet(std::move(newBullet));
}

void BossMainBody::AttackTypeALockon()
{
	//�^�C�}�[���X�V
	const float lockonTime = 120;
	attackATimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackATimer >= lockonTime) {
		attackAPhase = AttackTypeAPhase::Shot;

		//�^�C�}�[������
		attackATimer = 0;
	}
}

void BossMainBody::AttackTypeAShot()
{
	//�^�C�}�[���X�V
	const float shotTime = 380;
	attackATimer++;

	//�e���˃^�C�}�[�J�E���g
	fireTimer++;
	//���ˊԊu
	const int fireInterval = 10;
	if (fireTimer >= fireInterval) {
		//�e�𔭎�
		const float bulletScale = 1.0f;
		const float bulletSpeed = 1.5f;
		Fire(bulletScale, bulletSpeed);
		//���˃^�C�}�[��������
		fireTimer = 0;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackATimer >= shotTime) {
		attackAPhase = AttackTypeAPhase::Stay;

		//�^�C�}�[������
		attackATimer = 0;
	}
}

void BossMainBody::AttackTypeA2Move()
{
	//�^�C�}�[���X�V
	const float moveTime = 150;
	attackA2Timer++;
	const float time = attackA2Timer / moveTime;

	//���Ɉړ�������
	const float moveZ = 100;
	position.z = Easing::OutQuad(basePos.z, basePos.z + moveZ, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackA2Timer >= moveTime) {
		attackA2Phase = AttackTypeA2Phase::ChargeShot;

		//�^�C�}�[������
		attackA2Timer = 0;
	}
}

void BossMainBody::AttackTypeA2ChargeShot()
{
	//�^�C�}�[���X�V
	const float chargeTime = 300;
	attackA2Timer++;
	const float time = attackA2Timer / chargeTime;

	//�F��Ԃ�����
	color.y = Easing::OutQuad(1, 0, time);
	color.z = Easing::OutQuad(1, 0, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackA2Timer >= chargeTime) {
		attackA2Phase = AttackTypeA2Phase::Stay;

		//�^�C�}�[������
		attackA2Timer = 0;

		//�e�𔭎�
		const float bulletScale = 20.0f;
		const float bulletSpeed = 1.0f;
		Fire(bulletScale, bulletSpeed);

		//�F�����ɖ߂�
		color = { 1, 1, 1, 1 };
	}
}

void BossMainBody::AttackTypeA3Wait()
{
	//�^�C�}�[���X�V
	const float waitTime = 60;
	attackA3Timer++;
	const float time = attackA3Timer / waitTime;

	//�U�����e��������₷�悤��Z���Ɉ��]������
	rotation.z = Easing::OutQuad(0, 360, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackA3Timer >= waitTime) {
		attackA3Phase = AttackTypeA3Phase::Shot;

		//�^�C�}�[������
		attackA3Timer = 0;

		//�O�̈�Z����]��0�ɖ߂��Ă���
		rotation.z = 0;
	}
}

void BossMainBody::AttackTypeA3Shot()
{
	//�e�𔭎�
	const float bulletScale = 5.0f;
	const float bulletSpeed = 1.5f;
	Fire(bulletScale, bulletSpeed);

	//�U���񐔂��J�E���g
	attackA3ShotCount++;

	//�w�肵���񐔍U�����Ă��Ȃ���Ύ��̈ʒu�Ɉړ�������
	const int attackNum = 6;
	if (attackA3ShotCount < attackNum) {
		attackA3Phase = AttackTypeA3Phase::Move;

		//���̈ړ���̍��W���Z�b�g
		if (attackA3ShotCount == 1) { attackA3MpveAfterPos = { 10, 10, position.z }; }
		else if (attackA3ShotCount == 2) { attackA3MpveAfterPos = { -10, 10, position.z }; }
		else if (attackA3ShotCount == 3) { attackA3MpveAfterPos = { 10, -10, position.z }; }
		else if (attackA3ShotCount == 4) { attackA3MpveAfterPos = { -10, -10, position.z }; }
		else if (attackA3ShotCount == 5) { attackA3MpveAfterPos = { 0, 0, position.z }; }

		//���݂̍��W���ړ��O���W�Ƃ��ċL�^���Ă���
		attackA3MoveBeforePos = position;
	}
	//�w�肵���񐔍U��������ҋ@��Ԃɂ���
	else {
		attackA3Phase = AttackTypeA3Phase::Stay;
	}
}

void BossMainBody::AttackTypeA3Move()
{
	//�^�C�}�[���X�V
	const float moveTime = 60;
	attackA3Timer++;
	const float time = attackA3Timer / moveTime;

	//�w�肵���ʒu�Ɉړ�������
	const float moveZ = 100;
	position.x = Easing::OutQuad(attackA3MoveBeforePos.x, attackA3MpveAfterPos.x, time);
	position.y = Easing::OutQuad(attackA3MoveBeforePos.y, attackA3MpveAfterPos.y, time);

	//�U�����e��������₷�悤��Z���Ɉ��]������
	rotation.z = Easing::OutQuad(0, 360, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackA3Timer >= moveTime) {
		attackA3Phase = AttackTypeA3Phase::Shot;

		//�^�C�}�[������
		attackA3Timer = 0;

		//�O�̈�Z����]��0�ɖ߂��Ă���
		rotation.z = 0;
	}
}

void BossMainBody::Stay()
{
}
