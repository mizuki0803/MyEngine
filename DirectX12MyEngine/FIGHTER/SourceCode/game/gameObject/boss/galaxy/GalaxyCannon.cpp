#include "GalaxyCannon.h"
#include "Easing.h"
#include "BaseStageScene.h"
#include "BossChargeBullet.h"
#include "ParticleEmitter.h"
#include "EnemyBreakEffect.h"

void (GalaxyCannon::* GalaxyCannon::attackTypeRapidFirePhaseFuncTable[])() = {
	&GalaxyCannon::AttackTypeRapidFireStartWait,
	&GalaxyCannon::AttackTypeRapidFireCharge,
	&GalaxyCannon::AttackTypeRapidFireShot,
	&GalaxyCannon::AttackTypeRapidFireReturn,
	&GalaxyCannon::AttackTypeRapidFireWait,
};

BaseStageScene* GalaxyCannon::stageScene = nullptr;
ObjModel* GalaxyCannon::cannonModel = nullptr;
ObjModel* GalaxyCannon::cannonDeadModel = nullptr;
ObjModel* GalaxyCannon::bulletModel = nullptr;
std::array<ObjModel*, 5> GalaxyCannon::breakModels;
const Vector3 GalaxyCannon::normalSize = { 0.22f, 0.22f, 0.22f };
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

void GalaxyCannon::SetBreakModel(int modelNum, ObjModel* model)
{
	//�j�󎞂̃��f�����Z�b�g
	assert(model);
	breakModels[modelNum] = model;
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

		//�j��p���o��ǉ�
		Break();
		
		//����W�ɖ߂�
		position = basePos;

		//HP�Q�[�W�o�O���N�����Ȃ��悤�}�C�i�X����0�ɒ���
		damageNum += HP;

		//���S�p���f���ɕύX
		model = cannonDeadModel;

		//�������o�p�p�[�e�B�N������
		const float size = 5.0f;
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

void GalaxyCannon::AttackModeStart()
{
	//�U�����e:���˂̕ϐ��̏�����
	attackTypeRapidFirePhase = AttackTypeRapidFirePhase::StartWait;
	rapidFireCount = 0;

	//�U���Ŏg���^�C�}�[��������
	attackTimer = 0;

	//�U�����ɂ���
	isAttack = true;
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

void GalaxyCannon::AttackTypeRapidFireStartWait()
{
	//�^�C�}�[�X�V
	attackTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= rapidFireStartWaitTime) {
		attackTypeRapidFirePhase = AttackTypeRapidFirePhase::Charge;

		//�^�C�}�[�����������Ă���
		attackTimer = 0;
	}
}

void GalaxyCannon::AttackTypeRapidFireCharge()
{
	//�`���[�W�ɂ����鎞��
	const float chargeTime = 50;
	//�^�C�}�[�X�V
	attackTimer++;
	const float time = attackTimer / chargeTime;

	//�C�[�W���O�Ō��Ɉ�������
	const float easeStart = basePos.x;
	const float easeEnd = basePos.x - 0.3f;
	position.x = Easing::OutQuint(easeStart, easeEnd, time);

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
	const float shotModeTime = 5;
	//�^�C�}�[�X�V
	attackTimer++;
	const float time = attackTimer / shotModeTime;

	//�C�[�W���O�ň�C�ɉ����o��
	const float easeStart = basePos.x - 0.3f;
	const float easeEnd = basePos.x + 0.25f;
	position.x = Easing::OutQuint(easeStart, easeEnd, time);

	//���˂���^�C�~���O�ɂȂ����甭��
	const int shotTime = 3;
	if (attackTimer == shotTime) {
		//�e����
		const float bulletSize = 3.0f;
		const float bulletSpeed = 0.7f;
		Fire(bulletSize, bulletSpeed);

		//���ˉ񐔍X�V
		rapidFireCount++;
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
	const float returnTime = 10;
	//�^�C�}�[�X�V
	attackTimer++;
	const float time = attackTimer / returnTime;

	//�C�[�W���O�Ŗ߂�
	const float easeStart = basePos.x + 0.25f;
	const float easeEnd = basePos.x;
	position.x = Easing::InQuad(easeStart, easeEnd, time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= returnTime) {
		attackTypeRapidFirePhase = AttackTypeRapidFirePhase::Wait;

		//�^�C�}�[�����������Ă���
		attackTimer = 0;
	}
}

void GalaxyCannon::AttackTypeRapidFireWait()
{
	//�U�����łȂ���Δ�����
	if (!isAttack) { return; }

	//���Ɏw�肵���񐔔��˂��Ă�����U���I�������Ĕ�����
	const int rapidFireNum = 10;
	if (rapidFireCount >= rapidFireNum) {
		//�U����ԏI��
		isAttack = false;

		return;
	}

	//�ҋ@����
	const float waitTime = 20;
	//�^�C�}�[�X�V
	attackTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= waitTime) {
		attackTypeRapidFirePhase = AttackTypeRapidFirePhase::Charge;

		//�^�C�}�[�����������Ă���
		attackTimer = 0;
	}
}

void GalaxyCannon::Break()
{
	//�j��p�G�t�F�N�g�ǉ�
	for (int i = 0; i < breakModels.size(); i++) {
		//���f�������ݒ�Ȃ��΂�
		if (!breakModels[i]) { continue; }

		//�����_���ŃG�t�F�N�g�̑��x���Z�b�g
		const Vector3 randVel = { 4, 4, 4 };
		Vector3 velocity;
		velocity.x = (float)((rand() % (int)randVel.x) - randVel.x / 2);
		velocity.y = (float)((rand() % (int)randVel.y) - randVel.y / 2);
		velocity.z = -(float)((rand() % (int)randVel.z));

		//�����_���ŃG�t�F�N�g�̉�]�̑������Z�b�g
		const Vector3 randRotSpeed = { 5, 5, 5 };
		Vector3 rotSpeed;
		rotSpeed.x = (float)((rand() % (int)randRotSpeed.x) - randRotSpeed.x / 2);
		rotSpeed.y = (float)((rand() % (int)randRotSpeed.y) - randRotSpeed.y / 2);
		rotSpeed.z = (float)((rand() % (int)randRotSpeed.z) - randRotSpeed.z / 2);

		//�l���傫���̂Ŋ���Z���ď���������
		const float div = 2;
		velocity /= div;

		//�傫�����Z�b�g
		const Vector3 size = scale * 20;

		//�j��p�G�t�F�N�g�𐶐�
		std::unique_ptr<EnemyBreakEffect> newBreakEffect;
		newBreakEffect.reset(EnemyBreakEffect::Create(breakModels[i], bulletShotPos, velocity, rotSpeed, size));
		stageScene->AddEnemyBreakEffect(std::move(newBreakEffect));
	}
}
