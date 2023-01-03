#include "MedamanAvatar.h"
#include "MedamanMainBody.h"
#include "Easing.h"
#include "BaseStageScene.h"
#include "BossChargeBullet.h"
#include "ParticleEmitter.h"

void (MedamanAvatar::* MedamanAvatar::attackTypeAvatarBodyBlowPhaseFuncTable[])() = {
	&MedamanAvatar::AttackTypeAvatarBodyBlowLockon,
	&MedamanAvatar::AttackTypeAvatarBodyBlowShot,
	&MedamanAvatar::AttackTypeAvatarBodyBlowBack,
	&MedamanAvatar::Stay,
};

void (MedamanAvatar::* MedamanAvatar::attackTypeAvatarGatlingPhaseFuncTable[])() = {
	&MedamanAvatar::AttackTypeAvatarGatlingMoveCenter,
	&MedamanAvatar::AttackTypeAvatarGatlingRotStart,
	&MedamanAvatar::AttackTypeAvatarGatlingShot,
	&MedamanAvatar::AttackTypeAvatarGatlingRotEnd,
	&MedamanAvatar::Stay,
};

void (MedamanAvatar::* MedamanAvatar::attackTypeAvatarGiantBulletPhaseFuncTable[])() = {
	&MedamanAvatar::AttackTypeAvatarGiantBulletWait,
	&MedamanAvatar::AttackTypeAvatarGiantBulletMove,
	&MedamanAvatar::AttackTypeAvatarGiantBulletChargeShot,
	&MedamanAvatar::AttackTypeAvatarGiantBulletRecoil,
	&MedamanAvatar::Stay,
};


BaseStageScene* MedamanAvatar::stageScene = nullptr;
ObjModel* MedamanAvatar::avatarModel = nullptr;
ObjModel* MedamanAvatar::avatarDamageModel = nullptr;
ObjModel* MedamanAvatar::avatarSleepModel = nullptr;
ObjModel* MedamanAvatar::avatarDeadModel = nullptr;
ObjModel* MedamanAvatar::bulletModel = nullptr;
const Vector3 MedamanAvatar::normalSize = { 0.75f, 0.75f, 0.75f };
const Vector3 MedamanAvatar::damageSize = normalSize * 1.1f;
const float MedamanAvatar::attackModeRotY = 180.0f;
const float MedamanAvatar::waitModeRotY = 0.0f;
const float MedamanAvatar::attackAvatarGatlingLength = 1.25f;
const XMFLOAT4 MedamanAvatar::damageColor = { 1, 0.2f, 0.2f, 1 };

bool MedamanAvatar::Initialize()
{
	//�傫���͖{�̂�3/4
	scale = normalSize;

	//�I�u�W�F�N�g������
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	return true;
}

void MedamanAvatar::Update()
{
	//�_���[�W��Ԃ݂̂̏���
	if (isDamage) {
		DamageMode();
	}

	//���S���̓���
	if (isDead) {
		DeadAction();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void MedamanAvatar::Damage(int attackPower, const Vector3& collisionPos, const Vector3& subjectVel)
{
	//�����̍U���͂��_���[�W�ʂɃZ�b�g
	damageNum = attackPower;

	//�_���[�W��^����
	HP -= damageNum;

	//HP��0�ȉ��ɂȂ����玀�S
	if (HP <= 0) {
		Dead();

		//HP�Q�[�W�o�O���N�����Ȃ��悤�}�C�i�X����0�ɒ���
		damageNum += HP;

		//���S�p���f���ɕύX
		model = avatarDeadModel;

		//�������o�p�p�[�e�B�N������
		const float size = 3.0f;
		ParticleEmitter::GetInstance()->Explosion(GetWorldPos(), size);
	}

	//HP�����Ȃ���Ԃ̃��f�����Z�b�g����
	DamageModelChange();

	//�_���[�W��Ԃɂ���
	isDamage = true;
	//�_���[�W���������u�ԂȂ̂�true
	isDamageTrigger = true;
	//�_���[�W��ԃ^�C�}�[������
	damageTimer = 0;
	//�_���[�W��ԐF�ɕύX
	color = damageColor;

	//�T�C�Y�������傫������
	scale = damageSize;

	//�m�b�N�o�b�N�����Z�b�g
	SetDamageKnockback(subjectVel);

	//������������
	DamageExplosion(collisionPos);
}

void MedamanAvatar::DamageModelChange()
{
	//����HP�����Ȃ���Ԃ̃��f�����Z�b�g����Ă����甲����
	if (isDamageModel) { return; }

	//HP���w�肵���l�ȉ��łȂ��Ȃ甲����
	const int modelChangeHP = maxHP / 2;
	if (!(HP <= modelChangeHP)) { return; }

	//HP�����Ȃ���Ԃ̃��f�����Z�b�g����
	model = avatarDamageModel;
	//HP�����Ȃ���Ԃ̃��f�����Z�b�g����Ă����Ԃɂ���
	isDamageModel = true;
}

void MedamanAvatar::UpdateBulletShotPos()
{
	//���S���W����̋���
	const Vector3 distancePos = { 0, 0, scale.z * parent->GetScale().z - 1.0f };

	//�e���ˍ��W���擾
	bulletShotPos = LocalTranslation(distancePos, matWorld);
}

void MedamanAvatar::AttackTypeAvatarBodyBlow(const Vector3& playerPosition)
{
	//���@���W�����b�N�I������
	attackAvatarBodyBlowLockonPos = playerPosition;

	//�s��
	(this->*attackTypeAvatarBodyBlowPhaseFuncTable[static_cast<size_t>(attackAvatarBodyBlowPhase)])();
}

void MedamanAvatar::AttackTypeAvatarGatling()
{
	//�s��
	(this->*attackTypeAvatarGatlingPhaseFuncTable[static_cast<size_t>(attackAvatarGatlingPhase)])();
}

void MedamanAvatar::AttackTypeAvatarGiantBullet()
{
	//�s��
	(this->*attackTypeAvatarGiantBulletPhaseFuncTable[static_cast<size_t>(attackAvatarGiantBulletPhase)])();
}

void MedamanAvatar::ChangeAttackMode(const float time)
{
	//180�x��]�����Ĕ��Ό����ɂ���
	rotation.y = Easing::InOutBack(waitModeRotY, attackModeRotY, time);
}

void MedamanAvatar::ChangeWaitMode(const float time)
{
	//180�x��]�����Ĕ��Ό����ɂ���
	rotation.x = 0;
	rotation.y = Easing::InOutBack(attackModeRotY, waitModeRotY, time);
	rotation.z = 0;
}

void MedamanAvatar::SetReturnBasePosition()
{
	//�Ăяo�����u�Ԃ̍��W����ʒu�ɖ߂�Ƃ��̏o�����W�Ƃ��ċL�^���Ă���
	returnStartPos = position;
}

void MedamanAvatar::ReturnBasePosition(const float time)
{
	//�e�q�֌W��ł̊�ʒu���W�ɖ߂�
	position.x = Easing::OutQuad(returnStartPos.x, basePos.x, time);
	position.y = Easing::OutQuad(returnStartPos.y, basePos.y, time);
	position.z = Easing::OutQuad(returnStartPos.z, basePos.z, time);
}

void MedamanAvatar::AttackEnd()
{
	//�Ăяo�����u�Ԃ̍��W����ʒu�ɖ߂�Ƃ��̏o�����W�Ƃ��ċL�^���Ă���
	returnStartPos = position;

	//�e���˃^�C�}�[��������
	fireTimer = 0;

	//�U���^�C�}�[��������
	attackTimer = 0;

	//�U�����e:���g�̓�����̕ϐ��̏�����
	attackAvatarBodyBlowPhase = AttackTypeAvatarBodyBlowPhase::Lockon;

	//�U�����e:���g�K�g�����O�C�̕ϐ��̏�����
	attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::MoveCenter;
	attackAvatarGatlingRotSpeed = 0;

	//�U�����e:���g����e�̕ϐ��̏�����
	attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Wait;
	attackAvatarGiantBulletRecoilVelocity = { 0, 0, 2 };
	attackAvatarGiantBulletRecoilAccel = { 0, 0, -0.05f };
}

void MedamanAvatar::ChangeModel()
{
	//�N���Ă����Ԃ̃��f�����Z�b�g
	model = avatarModel;
}

Vector3 MedamanAvatar::GetWorldPos()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���s�ړ��������擾
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void MedamanAvatar::Fire(const float scale, const float bulletSpeed)
{
	//�e�̑��x��ݒ�
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, bulletShotPos, velocity, scale));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void MedamanAvatar::ChargeBulletFire(const float scale, const float bulletSpeed)
{
	//�e�̑��x��ݒ�
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//�`���[�W�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(BossChargeBullet::Create(bulletShotPos, velocity, scale));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void MedamanAvatar::DamageMode()
{
	//�_���[�W��Ԃ̎���
	const int damageTime = 20;
	damageTimer++;

	//�m�b�N�o�b�N
	DamageKnockback();

	//�傫�������T�C�Y��߂�
	DamageSizeReturn();

	//�_���[�W�F�؂�ւ�
	DamageColorChange();

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

void MedamanAvatar::SetDamageKnockback(const Vector3& subjectVel)
{
	//�m�b�N�o�b�N������������߂�(�Ώۂ̑��x�̕���)
	knockbackVec = subjectVel;
	knockbackVec.normalize();
}

void MedamanAvatar::DamageKnockback()
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

void MedamanAvatar::DamageSizeReturn()
{
	//�傫�������T�C�Y�����ɖ߂�����
	const float sizeReturnTime = 10;
	//�w�肵�����Ԉȏ�Ȃ甲����
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//�傫�������ɖ߂�
	scale = Easing::LerpVec3(damageSize, normalSize, time);

	//�e�����Ȃ��Ƃ��̓��_�}�[���{�̂̑傫���������Ă��ăX�P�[����������
	if (!parent) { scale *= MedamanMainBody::GetNormalSize().z; }
}

void MedamanAvatar::DamageExplosion(const Vector3& collisionPos)
{
	//�G�����ɉ��o���o�Ă��܂����Ƃ�����̂ŁA�G�̑傫���������߂�
	Vector3 pos = collisionPos;
	float avatarSize = scale.z;
	//�e������Ƃ��͐e�̃X�P�[����������
	if (parent) { avatarSize *= parent->GetScale().z; }
	pos.z -= avatarSize / 2;
	//�����_���ł��炷
	const Vector3 randPos = { 2, 2, 1 };
	pos.x += (float)((rand() % (int)randPos.x) - randPos.x / 2);
	pos.y += (float)((rand() % (int)randPos.y) - randPos.y / 2);
	pos.z += (float)((rand() % (int)randPos.z));

	//�V���b�g���S���o�p�p�[�e�B�N������
	const float size = 0.75f;
	ParticleEmitter::GetInstance()->Explosion(pos, size);
}

void MedamanAvatar::DamageColorChange()
{
	//�_���[�W�F�؂�ւ��Ԋu����
	const int colorChangeInterval = 2;

	//�^�C�}�[���w�肵���Ԋu�ȊO�Ȃ甲����
	if (damageTimer % colorChangeInterval != 0) { return; }

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

void MedamanAvatar::AttackTypeAvatarBodyBlowShot()
{
	//���b�N�I���Ō��߂����x�Ŕ�΂�
	position += attackAvatarBodyBlowVelocity;

	//�^�C�}�[���X�V
	const float shotTime = 180;
	attackTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= shotTime) {
		attackAvatarBodyBlowPhase = AttackTypeAvatarBodyBlowPhase::Back;

		//�^�C�}�[������
		attackTimer = 0;
	}
}

void MedamanAvatar::AttackTypeAvatarBodyBlowBack()
{
	//�^�C�}�[���X�V
	const float backTime = 120;
	attackTimer++;
	const float time = attackTimer / backTime;

	//�e�q�֌W��ł̊���W�ɖ߂�
	position.x = Easing::OutQuad(0, basePos.x, time);
	position.y = Easing::OutQuad(60, basePos.y, time);
	position.z = Easing::OutQuad(0, basePos.z, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= backTime) {
		attackAvatarBodyBlowPhase = AttackTypeAvatarBodyBlowPhase::Stay;

		//�^�C�}�[������
		attackTimer = 0;
	}
}

void MedamanAvatar::AttackTypeAvatarGatlingRotStart()
{
	//��]���J�n(��]���x�𑬂�����)
	const float rotAccelSpeed = 0.1f;
	attackAvatarGatlingRotSpeed += rotAccelSpeed;

	//�p�x���ʓx�@�ɕϊ�
	float radius = XMConvertToRadians(attackAvatarGatlingAngle);

	//�~�̒��S����w�肵�����a�̒��������邮����
	position.x = cos(radius) * attackAvatarGatlingLength;
	position.y = sin(radius) * attackAvatarGatlingLength;

	//�p�x�̍X�V
	attackAvatarGatlingAngle += attackAvatarGatlingRotSpeed;

	//��]���x���w�肵�����x�ȏ�ɂȂ����玟�̃t�F�[�Y��
	const float maxRotSpeed = 4.0f;
	if (attackAvatarGatlingRotSpeed >= maxRotSpeed) {
		attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::Shot;
	}
}

void MedamanAvatar::AttackTypeAvatarGatlingShot()
{
	//�^�C�}�[���X�V
	const float moveTime = 300;
	attackTimer++;

	//�p�x���ʓx�@�ɕϊ�
	float radius = XMConvertToRadians(attackAvatarGatlingAngle);

	//�~�̒��S����w�肵�����a�̒��������邮����
	position.x = cos(radius) * attackAvatarGatlingLength;
	position.y = sin(radius) * attackAvatarGatlingLength;

	//�p�x�̍X�V
	attackAvatarGatlingAngle += attackAvatarGatlingRotSpeed;

	//�e���˃^�C�}�[�J�E���g
	fireTimer++;
	const int fireInterval = 10;
	if (fireTimer >= fireInterval) {
		//�e�𔭎�
		const float bulletScale = 1.0f;
		const float bulletSpeed = 0.75f;
		Fire(bulletScale, bulletSpeed);
		//���˃^�C�}�[��������
		fireTimer = 0;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= moveTime) {
		attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::RotEnd;

		//�^�C�}�[������
		attackTimer = 0;
	}
}

void MedamanAvatar::AttackTypeAvatarGatlingRotEnd()
{
	//��]���I��(��]���x��x������)
	const float rotAccelSpeed = -0.1f;
	attackAvatarGatlingRotSpeed += rotAccelSpeed;

	//�p�x���ʓx�@�ɕϊ�
	float radius = XMConvertToRadians(attackAvatarGatlingAngle);

	//�~�̒��S����w�肵�����a�̒��������邮����
	position.x = cos(radius) * attackAvatarGatlingLength;
	position.y = sin(radius) * attackAvatarGatlingLength;

	//�p�x�̍X�V
	attackAvatarGatlingAngle += attackAvatarGatlingRotSpeed;

	//��]���x��0�ȉ��ɂȂ����玟�̃t�F�[�Y��
	if (attackAvatarGatlingRotSpeed <= 0) {
		attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::Stay;
	}
}

void MedamanAvatar::AttackTypeAvatarGiantBulletChargeShot()
{
	//�^�C�}�[���X�V
	const float chargeTime = 180;
	attackTimer++;
	const float time = attackTimer / chargeTime;

	//�F��Ԃ�����
	color.y = Easing::OutQuad(1, 0, time);
	color.z = Easing::OutQuad(1, 0, time);

	//���˂���e�̑傫��
	const float bulletScale = 8.0f;
	//�`���[�W���o
	const float effectScale = Easing::OutCubic(0, bulletScale, time);
	ParticleEmitter::GetInstance()->BossCharge(bulletShotPos, effectScale);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= chargeTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Recoil;

		//�^�C�}�[������
		attackTimer = 0;

		//�e�𔭎�
		const float bulletSpeed = 0.8f;
		ChargeBulletFire(bulletScale, bulletSpeed);

		//�F�����ɖ߂�
		color = { 1, 1, 1, 1 };
	}
}

void MedamanAvatar::AttackTypeAvatarGiantBulletRecoil()
{
	//�����ŉ��Ɉړ�������
	position += attackAvatarGiantBulletRecoilVelocity;
	attackAvatarGiantBulletRecoilVelocity += attackAvatarGiantBulletRecoilAccel;

	//�����̑��x��0�ɂȂ����玟�̃t�F�[�Y��
	if (attackAvatarGiantBulletRecoilVelocity.z <= 0) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Stay;
	}
}

void MedamanAvatar::Stay()
{
}

void MedamanAvatar::Dead()
{
	//���S��Ԃɂ���
	isDead = true;

	//���S���Ă���̂ɐe�ɍ��킹�ĉ�]�Ȃǂ����Ă��܂��ƈ�a��������̂ŁA�e�q�\������������
	//�e�q�\�������ɔ����A���W�Ƒ傫����ݒ肵����
	position = GetWorldPos();
	scale = { matWorld.r[0].m128_f32[0], matWorld.r[1].m128_f32[1], matWorld.r[2].m128_f32[2] };
	parent = nullptr;
}

void MedamanAvatar::DeadAction()
{
	//X����]�����Ȃ���ė�����
	const float rotSpeed = 1.0f;
	rotation.x += rotSpeed;

	//�ė����邽�߁A���x�ɉ����x��������
	Vector3 crashAccel = { 0, -0.005f, 0 };
	crashVel += crashAccel;
	//�������鑬�x�̍ő�l��ݒ�
	const float maxCrashSpeed = -0.4f;
	if (crashVel.y <= maxCrashSpeed) { crashVel.y = maxCrashSpeed; }
	position += crashVel;

	//�����p�[�e�B�N������
	DeadBlackSmoke();

	//Y���W��0�ȉ��ɂȂ�����폜
	if (GetWorldPos().y <= 0) {
		isDelete = true;

		//�������o�p�p�[�e�B�N������
		ParticleEmitter::GetInstance()->Explosion(GetWorldPos(), 3, 30);
	}
}

void MedamanAvatar::DeadBlackSmoke()
{
	//���S�^�C�}�[�X�V
	deadTimer++;
	//���S���Ă��獕�����o���܂ł̎���
	const int smokeStartTime = 30;

	//�^�C�}�[���������o���܂ł̎��Ԉȉ��Ȃ甲����
	if (deadTimer < smokeStartTime) { return; }

	//���t���[���o���Ƒ����̂ŊԊu��ݒ�
	const int smokeInterval = 5;
	//�w�肵���Ԋu�ȊO�Ȃ甲����
	if (deadTimer % smokeInterval != 0) { return; }

	//�����p�[�e�B�N������
	const float smokeSize = scale.x * 1.5f;
	ParticleEmitter::GetInstance()->BlackSmoke(position, smokeSize);
}
