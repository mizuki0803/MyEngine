#include "Boss2Body.h"
#include "Easing.h"
#include "ParticleEmitter.h"

BaseStageScene* Boss2Body::stageScene = nullptr;
ObjModel* Boss2Body::bodyModel = nullptr;
const Vector3 Boss2Body::normalSize = { 4.5f, 4.5f, 4.5f };
const Vector3 Boss2Body::damageSize = Boss2Body::normalSize * 1.1f;
const XMFLOAT4 Boss2Body::damageColor = { 1, 0.2f, 0.2f, 1 };

Boss2Body* Boss2Body::Create(const Vector3& basePos)
{
	//���_�}�[��(�{��)�̃C���X�^���X�𐶐�
	Boss2Body* boss2Body = new Boss2Body();
	if (boss2Body == nullptr) {
		return nullptr;
	}

	//�Q�Ă����Ԃ̃��f�����Z�b�g
	assert(bodyModel);
	boss2Body->model = bodyModel;

	// ������
	if (!boss2Body->Initialize()) {
		delete boss2Body;
		assert(0);
		return nullptr;
	}

	//��~�����̍��W���Z�b�g
	boss2Body->basePos = basePos;

	//�傫�����Z�b�g
	boss2Body->scale = normalSize;

	return boss2Body;
}

void Boss2Body::Update()
{
	//�_���[�W��Ԃ݂̂̏���
	if (isDamage) {
		DamageMode();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void Boss2Body::Damage(int attackPower, const Vector3& collisionPos, const Vector3& subjectVel)
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

void Boss2Body::Fall(const float time)
{
	//��̈ʒu�̐^�ォ��~��Ă���
	Vector3 bornPos = basePos;
	const float fallNum = 70;
	bornPos.y = basePos.y + fallNum;
	position = Easing::LerpVec3(bornPos, basePos, time);
}

Vector3 Boss2Body::GetWorldPos()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���s�ړ��������擾
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void Boss2Body::Wait()
{
}

void Boss2Body::DamageMode()
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

void Boss2Body::SetDamageKnockback(const Vector3& subjectVel)
{
	//�m�b�N�o�b�N������������߂�(�Ώۂ̑��x�̕���)
	knockbackVec = subjectVel;
	knockbackVec.normalize();
}

void Boss2Body::DamageKnockback()
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

void Boss2Body::DamageSizeReturn()
{
	//�傫�������T�C�Y�����ɖ߂�����
	const float sizeReturnTime = 10;
	//�w�肵�����Ԉȏ�Ȃ甲����
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//�傫�������ɖ߂�
	scale = Easing::LerpVec3(damageSize, normalSize, time);
}

void Boss2Body::DamageExplosion(const Vector3& position)
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

void Boss2Body::DamageColorMode()
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
