#include "GalaxyBow.h"
#include "Easing.h"
#include "BaseStageScene.h"
#include "BossChargeBullet.h"
#include "EnemyFireBullet.h"
#include "ParticleEmitter.h"

void (GalaxyBow::* GalaxyBow::attackTypeFlamethrowerPhaseFuncTable[])() = {
	&GalaxyBow::AttackTypeFlamethrowerCharge,
	&GalaxyBow::AttackTypeFlamethrowerShot ,
	&GalaxyBow::AttackTypeFlamethrowerWait,
};

BaseStageScene* GalaxyBow::stageScene = nullptr;
ObjModel* GalaxyBow::bowModel = nullptr;
ObjModel* GalaxyBow::bowSleepModel = nullptr;
ObjModel* GalaxyBow::bowDeadModel = nullptr;
ObjModel* GalaxyBow::bulletModel = nullptr;
const Vector3 GalaxyBow::normalSize = { 0.25f, 0.25f, 0.25f };
const Vector3 GalaxyBow::damageSize = normalSize * 1.1f;
const XMFLOAT4 GalaxyBow::damageColor = { 1, 0.2f, 0.2f, 1 };


GalaxyBow* GalaxyBow::Create(ObjObject3d* parent, const Vector3& position)
{
	//�M�����N�V�[(�D��)�̃C���X�^���X�𐶐�
	GalaxyBow* galaxyBow = new GalaxyBow();
	if (galaxyBow == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(bowSleepModel);
	galaxyBow->model = bowSleepModel;

	// ������
	if (!galaxyBow->Initialize()) {
		delete galaxyBow;
		assert(0);
		return nullptr;
	}

	//�e���Z�b�g
	galaxyBow->parent = parent;

	//���W���Z�b�g
	galaxyBow->position = position;

	//�傫�����Z�b�g
	galaxyBow->scale = normalSize;

	return galaxyBow;
}

void GalaxyBow::Update()
{
	//�_���[�W��Ԃ̏���
	if (isDamage) {
		DamageMode();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void GalaxyBow::Damage(int attackPower, const Vector3& collisionPos)
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
		model = bowDeadModel;

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

void GalaxyBow::UpdateBulletShotPos()
{
	//���S���W����̋���
	const Vector3 distancePos = { 0, 0, -(scale.z * parent->GetScale().z) / 4 };

	//�e���ˍ��W���擾
	bulletShotPos = Vector3::LocalTranslation(distancePos, matWorld);
}

void GalaxyBow::AttackModeStart()
{
	//�U�����e:���˂̕ϐ��̏�����
	attackTypeFlamethrowerPhase = AttackTypeFlamethrowerPhase::Charge;

	//�U���Ŏg���^�C�}�[��������
	attackTimer = 0;

	//�U�����ɂ���
	isAttack = true;
}

void GalaxyBow::AttackTypeFlamethrower(const Vector3& playerPos)
{
	//�W�I�̍��W���X�V
	fireTargetPosition = playerPos;

	//�U�����e:�Ή����ˍs��
	(this->*attackTypeFlamethrowerPhaseFuncTable[static_cast<size_t>(attackTypeFlamethrowerPhase)])();
}

void GalaxyBow::ChangeModel()
{
	//�N���Ă����Ԃ̃��f�����Z�b�g
	model = bowModel;
}

void GalaxyBow::Fire(const float scale, const float bulletSpeed)
{
	//�e�̑��x��ݒ�
	Vector3 velocity(0, 0, -bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, bulletShotPos, velocity, scale));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void GalaxyBow::RockonFire(const float scale, const float bulletSpeed)
{
	//���W���W�I����O�Ȃ甭�˂��Ȃ�
	if (GetWorldPos().z <= fireTargetPosition.z) { return; }

	//�e�̑��x��ݒ�
	Vector3 DifferenceVec = fireTargetPosition - bulletShotPos;
	DifferenceVec.normalize();
	Vector3 velocity = DifferenceVec * bulletSpeed;

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyFireBullet::Create(bulletShotPos, velocity, scale));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void GalaxyBow::DamageMode()
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

void GalaxyBow::DamageSizeReturn()
{
	//�傫�������T�C�Y�����ɖ߂�����
	const float sizeReturnTime = 10;
	//�w�肵�����Ԉȏ�Ȃ甲����
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//�傫�������ɖ߂�
	scale = Easing::LerpVec3(damageSize, normalSize, time);
}

void GalaxyBow::DamageExplosion(const Vector3& collisionPos)
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

void GalaxyBow::DamageColorChange()
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

void GalaxyBow::AttackTypeFlamethrowerCharge()
{
	//�`���[�W�ɂ����鎞��
	const float chargeTime = 180;
	//�^�C�}�[�X�V
	attackTimer++;

	//����������Ԋu�̎���
	const int explosionInterval = 3;
	if (attackTimer % explosionInterval == 0) {
		//����
		//�D��̌����ɍ��킹��
		Vector3 pos = bulletShotPos;
		//�����_���ł��炷
		const Vector3 randPos = { 2, 2, 1 };
		pos.x += (float)((rand() % (int)randPos.x) - randPos.x / 2);
		pos.y += (float)((rand() % (int)randPos.y) - randPos.y / 2);
		pos.z += (float)((rand() % (int)randPos.z));

		//�����o�p�p�[�e�B�N������
		const float size = 0.5f;
		ParticleEmitter::GetInstance()->Explosion(pos, size);
	}

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= chargeTime) {
		attackTypeFlamethrowerPhase = AttackTypeFlamethrowerPhase::Shot;

		//�^�C�}�[�����������Ă���
		attackTimer = 0;
	}
}

void GalaxyBow::AttackTypeFlamethrowerShot()
{
	//���˂��鎞��
	const float shotModeTime = 450;
	//�^�C�}�[�X�V
	attackTimer++;

	//���˂���Ԋu�ɂȂ����甭��
	const int shotInterval = 10;
	if (attackTimer % shotInterval == 0) {
		//�e����
		const float bulletSize = 2.0f;
		const float bulletSpeed = 1.0f;
		RockonFire(bulletSize, bulletSpeed);
	}

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackTimer >= shotModeTime) {
		attackTypeFlamethrowerPhase = AttackTypeFlamethrowerPhase::Wait;

		//�U����ԏI��
		isAttack = false;

		//�^�C�}�[�����������Ă���
		attackTimer = 0;
	}
}

void GalaxyBow::AttackTypeFlamethrowerWait()
{
}
