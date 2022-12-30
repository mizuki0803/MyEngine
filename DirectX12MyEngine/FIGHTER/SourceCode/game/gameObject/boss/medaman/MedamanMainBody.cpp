#include "MedamanMainBody.h"
#include "Easing.h"
#include "BaseStageScene.h"
#include "BossChargeBullet.h"
#include "ParticleEmitter.h"

void (MedamanMainBody::* MedamanMainBody::attackTypeTrackingPhaseFuncTable[])() = {
	&MedamanMainBody::AttackTypeTrackingLockon,
	&MedamanMainBody::AttackTypeTrackingShot,
	&MedamanMainBody::Stay,
};

void (MedamanMainBody::* MedamanMainBody::attackTypeSuperGiantBulletPhaseFuncTable[])() = {
	&MedamanMainBody::AttackTypeSuperGiantBulletMove,
	&MedamanMainBody::AttackTypeSuperGiantBulletChargeShot,
	&MedamanMainBody::Stay,
};

void (MedamanMainBody::* MedamanMainBody::attackTypeRotatePhaseFuncTable[])() = {
	&MedamanMainBody::AttackTypeRotateWait,
	&MedamanMainBody::AttackTypeRotateShot,
	&MedamanMainBody::AttackTypeRotateMove,
	&MedamanMainBody::Stay,
};

BaseStageScene* MedamanMainBody::stageScene = nullptr;
ObjModel* MedamanMainBody::mainBodyModel = nullptr;
ObjModel* MedamanMainBody::mainBodyDamageModel = nullptr;
ObjModel* MedamanMainBody::mainBodySleepModel = nullptr;
ObjModel* MedamanMainBody::mainBodyDeadModel = nullptr;
ObjModel* MedamanMainBody::bulletModel = nullptr;
const Vector3 MedamanMainBody::normalSize = { 4.5f, 4.5f, 4.5f };
const Vector3 MedamanMainBody::damageSize = MedamanMainBody::normalSize * 1.1f;
const float MedamanMainBody::attackModeRotY = 180.0f;
const float MedamanMainBody::waitModeRotY = 0.0f;
const XMFLOAT4 MedamanMainBody::damageColor = { 1, 0.2f, 0.2f, 1 };

MedamanMainBody* MedamanMainBody::Create(const Vector3& basePos)
{
	//���_�}�[��(�{��)�̃C���X�^���X�𐶐�
	MedamanMainBody* medamanMainBody = new MedamanMainBody();
	if (medamanMainBody == nullptr) {
		return nullptr;
	}

	//�Q�Ă����Ԃ̃��f�����Z�b�g
	assert(mainBodySleepModel);
	medamanMainBody->model = mainBodySleepModel;

	// ������
	if (!medamanMainBody->Initialize()) {
		delete medamanMainBody;
		assert(0);
		return nullptr;
	}

	//��~�����̍��W���Z�b�g
	medamanMainBody->basePos = basePos;

	//�傫�����Z�b�g
	medamanMainBody->scale = normalSize;

	return medamanMainBody;
}

void MedamanMainBody::Update()
{
	//�_���[�W��Ԃ݂̂̏���
	if (isDamage) {
		DamageMode();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void MedamanMainBody::Damage(int attackPower, const Vector3& collisionPos, const Vector3& subjectVel)
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

		//���S���̍��W�Ɗp�x���L�����Ă���
		deadPos = position;
		deadRota = rotation;
	}

	//HP�����Ȃ���Ԃ̃��f�����Z�b�g����
	DamageModelChange();

	//�_���[�W��Ԃɂ���
	isDamage = true;
	//�_���[�W���������u�ԂȂ̂�true
	isDamageTrigger = true;
	//�_���[�W��ԃ^�C�}�[������
	damageTimer = 0;
	//�F��ύX
	color = damageColor;

	//�T�C�Y�������傫������
	scale = damageSize;

	//�m�b�N�o�b�N�����Z�b�g
	SetDamageKnockback(subjectVel);

	//������������
	DamageExplosion(collisionPos);
}

void MedamanMainBody::DamageModelChange()
{
	//����HP�����Ȃ���Ԃ̃��f�����Z�b�g����Ă����甲����
	if (isDamageModel) { return; }

	//HP���w�肵���l�ȉ��łȂ��Ȃ甲����
	const int modelChangeHP = maxHP / 2;
	if (!(HP <= modelChangeHP)) { return; }

	//HP�����Ȃ���Ԃ̃��f�����Z�b�g����
	model = mainBodyDamageModel;
	//HP�����Ȃ���Ԃ̃��f�����Z�b�g����Ă����Ԃɂ���
	isDamageModel = true;
}

void MedamanMainBody::Fall(const float time)
{
	//��̈ʒu�̐^�ォ��~��Ă���
	Vector3 bornPos = basePos;
	const float fallNum = 70;
	bornPos.y = basePos.y + fallNum;
	position = Easing::LerpVec3(bornPos, basePos, time);
}

void MedamanMainBody::UpdateBulletShotPos()
{
	//���S���W����̋���
	const Vector3 distancePos = { 0, 0, scale.z - 1.0f };
	//���s�ړ��s��̌v�Z
	XMMATRIX matTrans = XMMatrixTranslation(distancePos.x, distancePos.y, distancePos.z);

	//���[���h�s��̍���
	XMMATRIX bulletShotMatWorld;
	bulletShotMatWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	bulletShotMatWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f

	//���@�I�u�W�F�N�g�̃��[���h�s���������
	bulletShotMatWorld *= matWorld;

	//�e���ˍ��W���擾
	bulletShotPos = { bulletShotMatWorld.r[3].m128_f32[0], bulletShotMatWorld.r[3].m128_f32[1], bulletShotMatWorld.r[3].m128_f32[2] };
}

void MedamanMainBody::AttackTypeTracking(const Vector3& playerPosition)
{
	//��Ƀv���C���[�����Ɉړ�����
	Vector3 velocity = playerPosition - position;
	const float moveSpeed = 2.0f;
	velocity = velocity.normalize() * moveSpeed;
	position.x += velocity.x;
	position.y += velocity.y;

	//�n�ʂɂ߂荞�܂Ȃ��悤�ɂ���
	position.y = max(position.y, scale.y);

	//�s��
	(this->*attackTypeTrackingPhaseFuncTable[static_cast<size_t>(attackTrackingPhase)])();
}

void MedamanMainBody::AttackTypeSuperGiantBullet()
{
	//�s��
	(this->*attackTypeSuperGiantBulletPhaseFuncTable[static_cast<size_t>(attackSuperGiantBulletPhase)])();
}

void MedamanMainBody::AttackTypeRotate()
{
	//�s��
	(this->*attackTypeRotatePhaseFuncTable[static_cast<size_t>(attackRotatePhase)])();
}

void MedamanMainBody::AttackTypeAvatarBodyBlow()
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

void MedamanMainBody::AttackTypeAvatarGatling(const Vector3& playerPosition)
{
	//��Ƀv���C���[�����Ɉړ�����
	Vector3 velocity = playerPosition - position;
	const float moveSpeed = 1.0f;
	velocity = velocity.normalize() * moveSpeed;
	position.x += velocity.x;
	position.y += velocity.y;

	//�n�ʂɂ߂荞�܂Ȃ��悤�ɒ���
	const float moveLimitGround = 8.5f;
	position.y = max(position.y, moveLimitGround);
}

void MedamanMainBody::ChangeAttackMode(const float time)
{
	//180�x��]�����Ĕ��Ό����ɂ���
	rotation.y = Easing::InOutBack(waitModeRotY, attackModeRotY, time);
}

void MedamanMainBody::ChangeWaitMode(const float time)
{
	//180�x��]�����Ĕ��Ό����ɂ���
	rotation.x = 0;
	rotation.y = Easing::InOutBack(attackModeRotY, waitModeRotY, time);
	rotation.z = 0;
}

void MedamanMainBody::SetReturnBasePosition()
{
	//�Ăяo�����u�Ԃ̍��W����ʒu�ɖ߂�Ƃ��̏o�����W�Ƃ��ċL�^���Ă���
	returnStartPos = position;
}

void MedamanMainBody::ReturnBasePosition(const float time)
{
	//��̍��W�ɖ߂�
	position = Easing::LerpVec3(returnStartPos, basePos, time);
}

void MedamanMainBody::AttackEnd()
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

void MedamanMainBody::DeadBasePosBack(const float time)
{
	//�C�[�W���O�p�䗦��1���z���Ă����甲����
	if (time > 1.0f) { return; }

	//�C�[�W���O�ō��W�Ɗp�x����ɖ߂�
	position.x = Easing::OutQuad(deadPos.x, basePos.x, time);
	position.y = Easing::OutQuad(deadPos.y, basePos.y, time);
	position.z = Easing::OutQuad(deadPos.z, basePos.z, time);

	rotation.x = Easing::OutQuad(deadRota.x, 0, time);
	rotation.y = Easing::OutQuad(deadRota.y, 0, time);
	rotation.z = Easing::OutQuad(deadRota.z, 0, time);
}

void MedamanMainBody::DeadFall()
{
	//X����]�����Ȃ���ė�����
	const float rotSpeed = 0.5f;
	rotation.x += rotSpeed;

	//�ė����邽�߁A���x�ɉ����x��������
	Vector3 crashAccel = { 0, -0.0025f, 0 };
	deadFallVel += crashAccel;
	//�������鑬�x�̍ő�l��ݒ�
	const float maxCrashSpeed = -0.18f;
	if (deadFallVel.y <= maxCrashSpeed) { deadFallVel.y = maxCrashSpeed; }
	position += deadFallVel;

	//����������
	DeadFallExplosion();

	//Y���W��0�ȉ��ɂȂ�����폜
	if (GetWorldPos().y - (scale.y / 2) <= 0) {
		isDelete = true;
	}
}

void MedamanMainBody::ChangeModel()
{
	//�N���Ă����Ԃ̃��f�����Z�b�g
	model = mainBodyModel;
}

void MedamanMainBody::ChangeDeadModel()
{
	//���S�p���f���ɕύX
	model = mainBodyDeadModel;
}

Vector3 MedamanMainBody::GetWorldPos()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���s�ړ��������擾
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void MedamanMainBody::Fire(const float scale, const float bulletSpeed)
{
	//�e�̑��x��ݒ�
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, bulletShotPos, velocity, scale));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void MedamanMainBody::ChargeBulletFire(const float scale, const float bulletSpeed)
{
	//�e�̑��x��ݒ�
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//�`���[�W�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(BossChargeBullet::Create(bulletShotPos, velocity, scale));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void MedamanMainBody::DamageMode()
{
	//�_���[�W��Ԃ̎���
	const int damageTime = 20;
	damageTimer++;

	//�m�b�N�o�b�N
	DamageKnockback();

	//�傫�������T�C�Y��߂�
	DamageSizeReturn();

	//�_���[�W�F�؂�ւ�
	DamageColorMode();

	//�_���[�W�g���K�[�t���O��true�Ȃ牺�낵�Ă���
	if (isDamageTrigger) { isDamageTrigger = false; }

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (damageTimer >= damageTime) {
		//�_���[�W��Ԃ��I��
		isDamage = false;

		//�F�����ɖ߂��Ă���
		color = { 1, 1, 1, 1 };
	}
}

void MedamanMainBody::SetDamageKnockback(const Vector3& subjectVel)
{
	//�m�b�N�o�b�N������������߂�(�Ώۂ̑��x�̕���)
	knockbackVec = subjectVel;
	knockbackVec.normalize();
}

void MedamanMainBody::DamageKnockback()
{
	//�m�b�N�o�b�N���鎞��
	const float knockbackTime = 5;
	//�w�肵�����Ԉȏ�Ȃ甲����
	if (damageTimer > knockbackTime) { return; }

	const float time = damageTimer / knockbackTime;

	//���x���쐬
	const float knockbackBaseSpeed = 0.2f;
	knockbackVel = knockbackVec * (1 - time) * knockbackBaseSpeed;

	//���@���m�b�N�o�b�N������
	position += knockbackVel;
}

void MedamanMainBody::DamageSizeReturn()
{
	//�傫�������T�C�Y�����ɖ߂�����
	const float sizeReturnTime = 10;
	//�w�肵�����Ԉȏ�Ȃ甲����
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//�傫�������ɖ߂�
	scale = Easing::LerpVec3(damageSize, normalSize, time);
}

void MedamanMainBody::DamageExplosion(const Vector3& position)
{
	//�G�����ɉ��o���o�Ă��܂����Ƃ�����̂ŁA�G�̑傫���������߂�
	Vector3 pos = position;
	pos.z -= scale.z / 2;
	//�����_���ł��炷
	const Vector3 randPos = { 2, 2, 1 };
	pos.x += (float)((rand() % (int)randPos.x) - randPos.x / 2);
	pos.y += (float)((rand() % (int)randPos.y) - randPos.y / 2);
	pos.z += (float)((rand() % (int)randPos.z));

	//�V���b�g���S���o�p�p�[�e�B�N������
	const float size = 0.75f;
	ParticleEmitter::GetInstance()->Explosion(pos, size);
}

void MedamanMainBody::DamageColorMode()
{
	//�_���[�W�F�؂�ւ�����
	const int damageColorChangeTime = 2;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (damageTimer % damageColorChangeTime == 0) {
		//�_���[�W�F��Ԃ�؂�ւ���
		if (isDamageColor) {
			isDamageColor = false;

			//�F�����ɖ߂�
			color = { 1, 1, 1, 1 };
		}
		else {
			isDamageColor = true;

			//�_���[�W�F�ɂ���
			color = damageColor;
		}
	}
}

void MedamanMainBody::AttackTypeTrackingLockon()
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

void MedamanMainBody::AttackTypeTrackingShot()
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
		const float bulletScale = 1.5f;
		const float bulletSpeed = 1.2f;
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

void MedamanMainBody::AttackTypeSuperGiantBulletMove()
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

void MedamanMainBody::AttackTypeSuperGiantBulletChargeShot()
{
	//�^�C�}�[���X�V
	const float chargeTime = 300;
	attackTimer++;
	const float time = attackTimer / chargeTime;

	//�F��Ԃ�����
	color.y = Easing::OutQuad(1, 0, time);
	color.z = Easing::OutQuad(1, 0, time);

	//���˂���e�̑傫��
	const float bulletScale = 20.0f;
	//�`���[�W���o
	const float effectScale = Easing::OutCubic(0, bulletScale, time);
	ParticleEmitter::GetInstance()->BossCharge(bulletShotPos, effectScale);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= chargeTime) {
		attackSuperGiantBulletPhase = AttackTypeSuperGiantBulletPhase::Stay;

		//�^�C�}�[������
		attackTimer = 0;

		//�e�𔭎�
		const float bulletSpeed = 1.0f;
		ChargeBulletFire(bulletScale, bulletSpeed);

		//�F�����ɖ߂�
		color = { 1, 1, 1, 1 };
	}
}

void MedamanMainBody::AttackTypeRotateWait()
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

void MedamanMainBody::AttackTypeRotateShot()
{
	//�e�𔭎�
	const float bulletScale = 5.0f;
	const float bulletSpeed = 1.0f;
	Fire(bulletScale, bulletSpeed);

	//�U���񐔂��J�E���g
	attackRotateShotCount++;

	//�w�肵���񐔍U�����Ă��Ȃ���Ύ��̈ʒu�Ɉړ�������
	const int attackNum = 6;
	if (attackRotateShotCount < attackNum) {
		attackRotatePhase = AttackTypeRotatePhase::Move;

		//���̈ړ���̍��W���Z�b�g
		if (attackRotateShotCount == 1) { attackRotateMpveAfterPos = { basePos.x + 10, basePos.y + 10, position.z }; }
		else if (attackRotateShotCount == 2) { attackRotateMpveAfterPos = { basePos.x - 10, basePos.y + 10, position.z }; }
		else if (attackRotateShotCount == 3) { attackRotateMpveAfterPos = { basePos.x + 10, basePos.y - 10, position.z }; }
		else if (attackRotateShotCount == 4) { attackRotateMpveAfterPos = { basePos.x - 10, basePos.y - 10, position.z }; }
		else if (attackRotateShotCount == 5) { attackRotateMpveAfterPos = { basePos.x, basePos.y, position.z }; }

		//���݂̍��W���ړ��O���W�Ƃ��ċL�^���Ă���
		attackRotateMoveBeforePos = position;
	}
	//�w�肵���񐔍U��������ҋ@��Ԃɂ���
	else {
		attackRotatePhase = AttackTypeRotatePhase::Stay;
	}
}

void MedamanMainBody::AttackTypeRotateMove()
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

void MedamanMainBody::DeadFallExplosion()
{
	//����������Ԋu�̎���
	const int explosionInterval = 3;
	//�^�C�}�[���X�V
	deadFallExplosionTimer++;

	//�w�肵���Ԋu�ȊO�Ȃ甲����
	if (deadFallExplosionTimer % explosionInterval != 0) { return; }

	//����
	//�G�����ɉ��o���o�Ă��܂����Ƃ�����̂ŁA�G�̑傫���������߂�
	Vector3 pos = position;
	pos.z -= scale.z / 2;
	//�����_���ł��炷
	const Vector3 randPos = { 2, 2, 1 };
	pos.x += (float)((rand() % (int)randPos.x) - randPos.x / 2);
	pos.y += (float)((rand() % (int)randPos.y) - randPos.y / 2);
	pos.z += (float)((rand() % (int)randPos.z));

	//���S�������o�p�p�[�e�B�N������
	const float size = 1.0f;
	ParticleEmitter::GetInstance()->Explosion(pos, size);
}

void MedamanMainBody::Stay()
{
}
