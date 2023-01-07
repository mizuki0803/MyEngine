#include "GalaxyCannon.h"
#include "Easing.h"
#include "BaseStageScene.h"
#include "BossChargeBullet.h"
#include "ParticleEmitter.h"

void (GalaxyCannon::* GalaxyCannon::attackTypeRapidFirePhaseFuncTable[])() = {
	&GalaxyCannon::AttackTypeRapidFireCharge,
	&GalaxyCannon::AttackTypeRapidFireShot,
	&GalaxyCannon::AttackTypeRapidFireReturn,
};

BaseStageScene* GalaxyCannon::stageScene = nullptr;
ObjModel* GalaxyCannon::cannonModel = nullptr;
ObjModel* GalaxyCannon::cannonDeadModel = nullptr;
ObjModel* GalaxyCannon::bulletModel = nullptr;
const Vector3 GalaxyCannon::normalSize = { 0.25f, 0.25f, 0.25f };
const Vector3 GalaxyCannon::damageSize = normalSize * 1.1f;
const XMFLOAT4 GalaxyCannon::damageColor = { 1, 0.2f, 0.2f, 1 };

GalaxyCannon* GalaxyCannon::Create(ObjObject3d* parent, const Vector3& basePos)
{
	//�M�����N�V�[(��C)�̃C���X�^���X�𐶐�
	GalaxyCannon* galaxyCannon = new GalaxyCannon();
	if (galaxyCannon == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(cannonModel);
	galaxyCannon->model = cannonModel;

	// ������
	if (!galaxyCannon->Initialize()) {
		delete galaxyCannon;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	galaxyCannon->parent = parent;
	//����W���Z�b�g
	galaxyCannon->basePos = basePos;
	//���W���Z�b�g
	galaxyCannon->position = basePos;
	//�傫�����Z�b�g
	galaxyCannon->scale = normalSize;
	//�p�x���Z�b�g(�E����������)
	galaxyCannon->rotation = { 0, 270, 0 };

	return galaxyCannon;
}

void GalaxyCannon::Update()
{
	//�_���[�W��Ԃ̏���
	if (isDamage) {
		DamageMode();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void GalaxyCannon::Damage(int attackPower, const Vector3& collisionPos)
{
	//�����̍U���͂��_���[�W�ʂɃZ�b�g
	damageNum = attackPower;

	//�_���[�W��^����
	HP -= damageNum;

	//HP��0�ȉ��ɂȂ����玀�S
	if (HP <= 0) {
		//���S��Ԃɂ���
		isDead = true;

		//HP�Q�[�W�o�O���N�����Ȃ��悤�}�C�i�X����0�ɒ���
		damageNum += HP;

		//���S�p���f���ɕύX
		model = cannonDeadModel;

		//�������o�p�p�[�e�B�N������
		const float size = 3.0f;
		ParticleEmitter::GetInstance()->Explosion(GetWorldPos(), size);
	}

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

	//������������
	DamageExplosion(collisionPos);
}

void GalaxyCannon::UpdateBulletShotPos()
{
	//���S���W����̋���
	const Vector3 distancePos = { 0, 0, scale.z * parent->GetScale().z - 1.0f };

	//�e���ˍ��W���擾
	bulletShotPos = LocalTranslation(distancePos, matWorld);
}

void GalaxyCannon::AttackTypeRapidFire()
{
	//�U�����e:���ˍs��
	(this->*attackTypeRapidFirePhaseFuncTable[static_cast<size_t>(attackTypeRapidFirePhase)])();
}

void GalaxyCannon::Fire(const float scale, const float bulletSpeed)
{
	//�e�̑��x��ݒ�
	Vector3 velocity(0, 0, -bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, bulletShotPos, velocity, scale));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void GalaxyCannon::ChargeBulletFire(const float scale, const float bulletSpeed)
{
	//�e�̑��x��ݒ�
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//�`���[�W�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(BossChargeBullet::Create(bulletShotPos, velocity, scale));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void GalaxyCannon::DamageMode()
{
	//�_���[�W��Ԃ̎���
	const int damageTime = 20;
	damageTimer++;

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

void GalaxyCannon::DamageSizeReturn()
{
	//�傫�������T�C�Y�����ɖ߂�����
	const float sizeReturnTime = 10;
	//�w�肵�����Ԉȏ�Ȃ甲����
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//�傫�������ɖ߂�
	scale = Easing::LerpVec3(damageSize, normalSize, time);
}

void GalaxyCannon::DamageExplosion(const Vector3& collisionPos)
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

	//�������o�p�p�[�e�B�N������
	const float size = 0.75f;
	ParticleEmitter::GetInstance()->Explosion(pos, size);
}

void GalaxyCannon::DamageColorChange()
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

void GalaxyCannon::AttackModeStart()
{
	//�U�����e:���g�̓�����̕ϐ��̏�����
	attackTypeRapidFirePhase = AttackTypeRapidFirePhase::Charge;
	//�U���Ŏg���^�C�}�[��������
	attackTimer = 0;

	//�U�����ɂ���
	isAttack = true;
}

void GalaxyCannon::AttackTypeRapidFireCharge()
{
	//�`���[�W�ɂ����鎞��
	const float chargeTime = 60;
	//�^�C�}�[�X�V
	attackTimer++;
	const float time = attackTimer / chargeTime;

	//�C�[�W���O�Ō��Ɉ�������
	const float easeStart = basePos.x;
	const float easeEnd = basePos.x - 0.5f;
	position.x = Easing::OutCubic(easeStart, easeEnd, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= chargeTime) {
		attackTypeRapidFirePhase = AttackTypeRapidFirePhase::Shot;

		//�^�C�}�[�����������Ă���
		attackTimer = 0;
	}
}

void GalaxyCannon::AttackTypeRapidFireShot()
{
	//���˂ɂ����鎞��
	const float shotModeTime = 15;
	//�^�C�}�[�X�V
	attackTimer++;
	const float time = attackTimer / shotModeTime;

	//�C�[�W���O�ň�C�ɉ����o��
	const float easeStart = basePos.x - 0.5f;
	const float easeEnd = basePos.x + 0.25f;
	position.x = Easing::OutQuint(easeStart, easeEnd, time);

	//���˂���^�C�~���O�ɂȂ����甭��
	const int shotTime = 10;
	if (attackTimer == shotTime) {
		//�e����
		const float bulletSize = 1.5f;
		const float bulletSpeed = 2.0f;
		Fire(bulletSize, bulletSpeed);
	}

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= shotModeTime) {
		attackTypeRapidFirePhase = AttackTypeRapidFirePhase::Return;

		//�^�C�}�[�����������Ă���
		attackTimer = 0;
	}
}

void GalaxyCannon::AttackTypeRapidFireReturn()
{
	//�߂��̂ɂ����鎞��
	const float returnTime = 30;
	//�^�C�}�[�X�V
	attackTimer++;
	const float time = attackTimer / returnTime;

	//�C�[�W���O�Ŗ߂�
	const float easeStart = basePos.x + 0.25f;
	const float easeEnd = basePos.x;
	position.x = Easing::InQuad(easeStart, easeEnd, time);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (attackTimer >= returnTime) {
		//�U����ԏI��
		isAttack = false;

		//�^�C�}�[�����������Ă���
		attackTimer = 0;
	}
}
