#include "BossAvatar.h"
#include "Easing.h"
#include "GameScene.h"
#include "BossChargeBullet.h"
#include "ParticleEmitter.h"

void (BossAvatar::* BossAvatar::attackTypeAvatarBodyBlowPhaseFuncTable[])() = {
	&BossAvatar::AttackTypeAvatarBodyBlowLockon,
	&BossAvatar::AttackTypeAvatarBodyBlowShot,
	&BossAvatar::AttackTypeAvatarBodyBlowBack,
	&BossAvatar::Stay,
};

void (BossAvatar::* BossAvatar::attackTypeAvatarGatlingPhaseFuncTable[])() = {
	&BossAvatar::AttackTypeAvatarGatlingMoveCenter,
	&BossAvatar::AttackTypeAvatarGatlingRotStart,
	&BossAvatar::AttackTypeAvatarGatlingShot,
	&BossAvatar::AttackTypeAvatarGatlingRotEnd,
	&BossAvatar::Stay,
};

void (BossAvatar::* BossAvatar::attackTypeAvatarGiantBulletPhaseFuncTable[])() = {
	&BossAvatar::AttackTypeAvatarGiantBulletWait,
	&BossAvatar::AttackTypeAvatarGiantBulletMove,
	&BossAvatar::AttackTypeAvatarGiantBulletChargeShot,
	&BossAvatar::AttackTypeAvatarGiantBulletRecoil,
	&BossAvatar::Stay,
};


GameScene* BossAvatar::gameScene = nullptr;
ObjModel* BossAvatar::avatarModel = nullptr;
ObjModel* BossAvatar::avatarDamageModel = nullptr;
ObjModel* BossAvatar::avatarSleepModel = nullptr;
ObjModel* BossAvatar::bulletModel = nullptr;
const float BossAvatar::attackModeRotY = 180.0f;
const float BossAvatar::waitModeRotY = 0.0f;
const float BossAvatar::attackAvatarGatlingLength = 1.25f;

bool BossAvatar::Initialize()
{
	//�傫���͖{�̂�3/4
	scale = { 0.75f, 0.75f, 0.75f };

	//�I�u�W�F�N�g������
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	return true;
}

void BossAvatar::Update()
{
	//�_���[�W�F��Ԃ݂̂̏���
	if (isDamageColor) {
		DamageColorMode();
	}

	//���S���̓���
	if (isDead) {
		DeadAction();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void BossAvatar::Damage(int attackPower)
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

		//�������o�p�p�[�e�B�N������
		ParticleEmitter::GetInstance()->Explosion(GetWorldPos());
	}

	//HP�����Ȃ���Ԃ̃��f�����Z�b�g����
	DamageModelChange();

	//�_���[�W�F��Ԃɂ���
	isDamageColor = true;
	const XMFLOAT4 damageColor = { 1, 0, 0, 1 };
	color = damageColor;
	//�_���[�W�F��ԃ^�C�}�[������
	damageColorTimer = 0;
}

void BossAvatar::DamageModelChange()
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

void BossAvatar::UpdateBulletShotPos()
{
	//���S���W����̋���
	const Vector3 distancePos = { 0, 0, scale.z * parent->GetScale().z - 1.0f };
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

void BossAvatar::AttackTypeAvatarBodyBlow(const Vector3& playerPosition)
{
	//���@���W�����b�N�I������
	attackAvatarBodyBlowLockonPos = playerPosition;

	//�s��
	(this->*attackTypeAvatarBodyBlowPhaseFuncTable[static_cast<size_t>(attackAvatarBodyBlowPhase)])();
}

void BossAvatar::AttackTypeAvatarGatling()
{
	//�s��
	(this->*attackTypeAvatarGatlingPhaseFuncTable[static_cast<size_t>(attackAvatarGatlingPhase)])();
}

void BossAvatar::AttackTypeAvatarGiantBullet()
{
	//�s��
	(this->*attackTypeAvatarGiantBulletPhaseFuncTable[static_cast<size_t>(attackAvatarGiantBulletPhase)])();
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

void BossAvatar::ChangeModel()
{
	//�N���Ă����Ԃ̃��f�����Z�b�g
	model = avatarModel;
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
	newBullet.reset(EnemyBullet::Create(bulletModel, bulletShotPos, velocity, scale));
	gameScene->AddEnemyBullet(std::move(newBullet));
}

void BossAvatar::ChargeBulletFire(const float scale, const float bulletSpeed)
{
	//�e�̑��x��ݒ�
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//�`���[�W�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(BossChargeBullet::Create(bulletShotPos, velocity, scale));
	gameScene->AddEnemyBullet(std::move(newBullet));
}

void BossAvatar::DamageColorMode()
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

void BossAvatar::AttackTypeAvatarBodyBlowShot()
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

void BossAvatar::AttackTypeAvatarBodyBlowBack()
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

void BossAvatar::AttackTypeAvatarGatlingRotStart()
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

void BossAvatar::AttackTypeAvatarGatlingShot()
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

void BossAvatar::AttackTypeAvatarGatlingRotEnd()
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

void BossAvatar::AttackTypeAvatarGiantBulletChargeShot()
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

void BossAvatar::AttackTypeAvatarGiantBulletRecoil()
{
	//�����ŉ��Ɉړ�������
	position += attackAvatarGiantBulletRecoilVelocity;
	attackAvatarGiantBulletRecoilVelocity += attackAvatarGiantBulletRecoilAccel;

	//�����̑��x��0�ɂȂ����玟�̃t�F�[�Y��
	if (attackAvatarGiantBulletRecoilVelocity.z <= 0) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Stay;
	}
}

void BossAvatar::Stay()
{
}

void BossAvatar::Dead()
{
	//���S��Ԃɂ���
	isDead = true;

	//���S���Ă���̂ɐe�ɍ��킹�ĉ�]�Ȃǂ����Ă��܂��ƈ�a��������̂ŁA�e�q�\������������
	//�e�q�\�������ɔ����A���W�Ƒ傫����ݒ肵����
	position = GetWorldPos();
	scale = { matWorld.r[0].m128_f32[0], matWorld.r[1].m128_f32[1], matWorld.r[2].m128_f32[2] };
	parent = nullptr;
}

void BossAvatar::DeadAction()
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

	//Y���W��0�ȉ��ɂȂ�����폜
	if (GetWorldPos().y <= 0) {
		isDelete = true;

		//�������o�p�p�[�e�B�N������
		ParticleEmitter::GetInstance()->Explosion(GetWorldPos());
	}
}