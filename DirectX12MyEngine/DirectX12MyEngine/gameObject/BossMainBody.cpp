#include "BossMainBody.h"
#include "Easing.h"
#include "GameScene.h"

void (BossMainBody::* BossMainBody::attackTypeTrackingPhaseFuncTable[])() = {
	&BossMainBody::AttackTypeTrackingLockon,
	&BossMainBody::AttackTypeTrackingShot,
	&BossMainBody::Stay,
};

void (BossMainBody::* BossMainBody::attackTypeSuperGiantBulletPhaseFuncTable[])() = {
	&BossMainBody::AttackTypeSuperGiantBulletMove,
	&BossMainBody::AttackTypeSuperGiantBulletChargeShot,
	&BossMainBody::Stay,
};

void (BossMainBody::* BossMainBody::attackTypeRotatePhaseFuncTable[])() = {
	&BossMainBody::AttackTypeRotateWait,
	&BossMainBody::AttackTypeRotateShot,
	&BossMainBody::AttackTypeRotateMove,
	&BossMainBody::Stay,
};

GameScene* BossMainBody::gameScene = nullptr;
ObjModel* BossMainBody::mainBodyModel = nullptr;
ObjModel* BossMainBody::mainBodySleepModel = nullptr;
ObjModel* BossMainBody::bulletModel = nullptr;
const float BossMainBody::attackModeRotY = 180.0f;
const float BossMainBody::waitModeRotY = 0.0f;

BossMainBody* BossMainBody::Create(const Vector3& basePos)
{
	//�{�X(�{��)�̃C���X�^���X�𐶐�
	BossMainBody* bossMainBody = new BossMainBody();
	if (bossMainBody == nullptr) {
		return nullptr;
	}

	//�Q�Ă����Ԃ̃��f�����Z�b�g
	assert(mainBodySleepModel);
	bossMainBody->model = mainBodySleepModel;

	// ������
	if (!bossMainBody->Initialize()) {
		delete bossMainBody;
		assert(0);
		return nullptr;
	}

	//��~�����̍��W���Z�b�g
	bossMainBody->basePos = basePos;

	//�傫�����Z�b�g
	bossMainBody->scale = { 4.5f, 4.5f, 4.5f };

	return bossMainBody;
}

void BossMainBody::Update()
{
	//�_���[�W�F��Ԃ݂̂̏���
	if (isDamageColor) {
		DamageColorMode();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void BossMainBody::Damage(int attackPower)
{
	//�����̍U���͂��_���[�W�ʂɃZ�b�g
	damageNum = attackPower;

	//�_���[�W��^����
	HP -= damageNum;

	//HP��0�ȉ��ɂȂ����玀�S
	if (HP <= 0) {
		isDead = true;

		//HP�Q�[�W�o�O���N�����Ȃ��悤�}�C�i�X����0�ɒ���
		damageNum += HP;
	}

	//�_���[�W�F��Ԃɂ���
	isDamageColor = true;
	const XMFLOAT4 damageColor = { 1, 0, 0, 1 };
	color = damageColor;
	//�_���[�W�F��ԃ^�C�}�[������
	damageColorTimer = 0;
}

void BossMainBody::FallMode(const float time)
{
	//��̈ʒu�̐^�ォ��~��Ă���
	Vector3 bornPos = basePos;
	const float fallNum = 70;
	bornPos.y = basePos.y + fallNum;
	position = Easing::Lerp(bornPos, basePos, time);
}

void BossMainBody::AttackTypeTracking(const Vector3& playerPosition)
{
	//��Ƀv���C���[�����Ɉړ�����
	Vector3 velocity = playerPosition - position;
	const float moveSpeed = 2.0f;
	velocity = velocity.normalize() * moveSpeed;
	position.x += velocity.x;
	position.y += velocity.y;

	//�s��
	(this->*attackTypeTrackingPhaseFuncTable[static_cast<size_t>(attackTrackingPhase)])();
}

void BossMainBody::AttackTypeSuperGiantBullet()
{
	//�s��
	(this->*attackTypeSuperGiantBulletPhaseFuncTable[static_cast<size_t>(attackSuperGiantBulletPhase)])();
}

void BossMainBody::AttackTypeRotate()
{
	//�s��
	(this->*attackTypeRotatePhaseFuncTable[static_cast<size_t>(attackRotatePhase)])();
}

void BossMainBody::AttackTypeAvatarBodyBlow()
{
	//�^�C�}�[���ړ����鎞�Ԃ��z���Ă����甲����
	const float moveTime = 100;
	if (attackTimer >= moveTime) { return; }

	//�^�C�}�[���X�V
	attackTimer++;
	const float time = attackTimer / moveTime;

	//���Ɉړ�������
	const float moveNum = 30;
	position.z = Easing::OutQuad(basePos.z, basePos.z + moveNum, time);
}

void BossMainBody::AttackTypeAvatarGatling(const Vector3& playerPosition)
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

	//�U���^�C�}�[��������
	attackTimer = 0;

	//�U�����e:�Ǐ]�ϐ��̏�����
	attackTrackingPhase = AttackTypeTrackingPhase::Lockon;

	//�U�����e:������e�̕ϐ��̏�����
	attackSuperGiantBulletPhase = AttackTypeSuperGiantBulletPhase::Move;

	//�U�����e:��]�̕ϐ��̏�����
	attackRotatePhase = AttackTypeRotatePhase::Wait;
	attackRotateShotCount = 0;
	attackRotateMoveBeforePos = {};
	attackRotateMpveAfterPos = {};
}

void BossMainBody::DeadMode()
{
	//X����]������
	Vector3 rotVel = { -0.5f, 0, 0 };
	rotation += rotVel;
}

void BossMainBody::ChangeModel()
{
	//�N���Ă����Ԃ̃��f�����Z�b�g
	model = mainBodyModel;
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

void BossMainBody::DamageColorMode()
{
	//�_���[�W�F�ɂ��鎞��
	const float damageColorTime = 10;
	damageColorTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ�����_���[�W�F��Ԃ���������
	if (damageColorTimer >= damageColorTime) {
		//�F�����ɖ߂�
		isDamageColor = false;
		color = { 1,1,1,1 };
	}
}

void BossMainBody::AttackTypeTrackingLockon()
{
	//�^�C�}�[���X�V
	const float lockonTime = 120;
	attackTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= lockonTime) {
		attackTrackingPhase = AttackTypeTrackingPhase::Shot;

		//�^�C�}�[������
		attackTimer = 0;
	}
}

void BossMainBody::AttackTypeTrackingShot()
{
	//�^�C�}�[���X�V
	const float shotTime = 380;
	attackTimer++;

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
	if (attackTimer >= shotTime) {
		attackTrackingPhase = AttackTypeTrackingPhase::Stay;

		//�^�C�}�[������
		attackTimer = 0;
	}
}

void BossMainBody::AttackTypeSuperGiantBulletMove()
{
	//�^�C�}�[���X�V
	const float moveTime = 150;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//���Ɉړ�������
	const float moveZ = 100;
	position.z = Easing::OutQuad(basePos.z, basePos.z + moveZ, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= moveTime) {
		attackSuperGiantBulletPhase = AttackTypeSuperGiantBulletPhase::ChargeShot;

		//�^�C�}�[������
		attackTimer = 0;
	}
}

void BossMainBody::AttackTypeSuperGiantBulletChargeShot()
{
	//�^�C�}�[���X�V
	const float chargeTime = 300;
	attackTimer++;
	const float time = attackTimer / chargeTime;

	//�F��Ԃ�����
	color.y = Easing::OutQuad(1, 0, time);
	color.z = Easing::OutQuad(1, 0, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= chargeTime) {
		attackSuperGiantBulletPhase = AttackTypeSuperGiantBulletPhase::Stay;

		//�^�C�}�[������
		attackTimer = 0;

		//�e�𔭎�
		const float bulletScale = 20.0f;
		const float bulletSpeed = 1.0f;
		Fire(bulletScale, bulletSpeed);

		//�F�����ɖ߂�
		color = { 1, 1, 1, 1 };
	}
}

void BossMainBody::AttackTypeRotateWait()
{
	//�^�C�}�[���X�V
	const float waitTime = 60;
	attackTimer++;
	const float time = attackTimer / waitTime;

	//�U�����e��������₷�悤��Z���Ɉ��]������
	rotation.z = Easing::OutQuad(0, 360, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= waitTime) {
		attackRotatePhase = AttackTypeRotatePhase::Shot;

		//�^�C�}�[������
		attackTimer = 0;

		//�O�̈�Z����]��0�ɖ߂��Ă���
		rotation.z = 0;
	}
}

void BossMainBody::AttackTypeRotateShot()
{
	//�e�𔭎�
	const float bulletScale = 5.0f;
	const float bulletSpeed = 1.5f;
	Fire(bulletScale, bulletSpeed);

	//�U���񐔂��J�E���g
	attackRotateShotCount++;

	//�w�肵���񐔍U�����Ă��Ȃ���Ύ��̈ʒu�Ɉړ�������
	const int attackNum = 6;
	if (attackRotateShotCount < attackNum) {
		attackRotatePhase = AttackTypeRotatePhase::Move;

		//���̈ړ���̍��W���Z�b�g
		if (attackRotateShotCount == 1) { attackRotateMpveAfterPos = { 10, 10, position.z }; }
		else if (attackRotateShotCount == 2) { attackRotateMpveAfterPos = { -10, 10, position.z }; }
		else if (attackRotateShotCount == 3) { attackRotateMpveAfterPos = { 10, -10, position.z }; }
		else if (attackRotateShotCount == 4) { attackRotateMpveAfterPos = { -10, -10, position.z }; }
		else if (attackRotateShotCount == 5) { attackRotateMpveAfterPos = { 0, 0, position.z }; }

		//���݂̍��W���ړ��O���W�Ƃ��ċL�^���Ă���
		attackRotateMoveBeforePos = position;
	}
	//�w�肵���񐔍U��������ҋ@��Ԃɂ���
	else {
		attackRotatePhase = AttackTypeRotatePhase::Stay;
	}
}

void BossMainBody::AttackTypeRotateMove()
{
	//�^�C�}�[���X�V
	const float moveTime = 60;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//�w�肵���ʒu�Ɉړ�������
	const float moveZ = 100;
	position.x = Easing::OutQuad(attackRotateMoveBeforePos.x, attackRotateMpveAfterPos.x, time);
	position.y = Easing::OutQuad(attackRotateMoveBeforePos.y, attackRotateMpveAfterPos.y, time);

	//�U�����e��������₷�悤��Z���Ɉ��]������
	rotation.z = Easing::OutQuad(0, 360, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= moveTime) {
		attackRotatePhase = AttackTypeRotatePhase::Shot;

		//�^�C�}�[������
		attackTimer = 0;

		//�O�̈�Z����]��0�ɖ߂��Ă���
		rotation.z = 0;
	}
}

void BossMainBody::Stay()
{
}
