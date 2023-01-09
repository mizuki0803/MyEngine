#include "GalaxyBody.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (GalaxyBody::* GalaxyBody::appearPhaseFuncTable[])() = {
	&GalaxyBody::AppaerAdvance,
	&GalaxyBody::AppaerWait,
	&GalaxyBody::AppaerRotation,
};

void (GalaxyBody::* GalaxyBody::attackPartChangePhaseFuncTable[])() = {
	&GalaxyBody::AttackPartChangeWait,
	&GalaxyBody::AttackPartChangeRotation,
};


BaseStageScene* GalaxyBody::stageScene = nullptr;
ObjModel* GalaxyBody::bodyModel = nullptr;
const Vector3 GalaxyBody::normalSize = { 25, 25, 25 };
const Vector3 GalaxyBody::damageSize = GalaxyBody::normalSize * 1.05f;
const XMFLOAT4 GalaxyBody::damageColor = { 1, 0.2f, 0.2f, 1 };

GalaxyBody* GalaxyBody::Create(const Vector3& bornPos, const Vector3& basePos)
{
	//�M�����N�V�[(����)�̃C���X�^���X�𐶐�
	GalaxyBody* galaxyBody = new GalaxyBody();
	if (galaxyBody == nullptr) {
		return nullptr;
	}

	//�Q�Ă����Ԃ̃��f�����Z�b�g
	assert(bodyModel);
	galaxyBody->model = bodyModel;

	// ������
	if (!galaxyBody->Initialize()) {
		delete galaxyBody;
		assert(0);
		return nullptr;
	}

	//�������W���Z�b�g
	galaxyBody->position = bornPos;
	//��~���W���Z�b�g
	galaxyBody->basePos = basePos;
	//�傫�����Z�b�g
	galaxyBody->scale = normalSize;

	return galaxyBody;
}

void GalaxyBody::Update()
{
	//�_���[�W��Ԃ̏���
	if (isDamage) {
		DamageMode();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void GalaxyBody::Damage()
{
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

	//������������
	DamageExplosion();
}

void GalaxyBody::Appear()
{
	//�o��s��
	(this->*appearPhaseFuncTable[static_cast<size_t>(appearPhase)])();
}

void GalaxyBody::AttackPartChangeRotaStart(const float rotSpeed, const float changeRota)
{
	//��]���x���Z�b�g
	attackPartChangeRotSpeed = rotSpeed;
	//��]��̊p�x���Z�b�g
	attackPartChangeRota = changeRota;

	//��]���E����������
	if (rotSpeed >= 0) { isAttackPartChangeRotaRight = true; }
	else { isAttackPartChangeRotaRight = false; }

	//�U������p�[�c�ύX�̍s���t�F�[�Y��ҋ@�ɂ��Ă���
	attackPartChangePhase = AttackPartChangePhase::Wait;
	//�U������p�[�c�ύX�������Ԃɂ���
	isAttackPartChangeRota = true;
}

void GalaxyBody::AttackPartChange()
{
	//�s��
	(this->*attackPartChangePhaseFuncTable[static_cast<size_t>(attackPartChangePhase)])();
}

void GalaxyBody::Dead()
{
	//������ɉ�]������
	const float rotSpeed = 0.1f;
	rotation.x += rotSpeed;

	//�������Ɉړ�
	position.y -= 0.05f;
}

void GalaxyBody::DamageMode()
{
	//�_���[�W��Ԃ̎���
	const int damageTime = 20;
	damageTimer++;

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

void GalaxyBody::DamageSizeReturn()
{
	//�傫�������T�C�Y�����ɖ߂�����
	const float sizeReturnTime = 10;
	//�w�肵�����Ԉȏ�Ȃ甲����
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//�傫�������ɖ߂�
	scale = Easing::LerpVec3(damageSize, normalSize, time);
}

void GalaxyBody::DamageExplosion()
{
	//�G�����ɉ��o���o�Ă��܂����Ƃ�����̂ŁA�G�̑傫���������߂�
	Vector3 pos = position;
	pos.z -= scale.z / 2;
	//�����_���ł��炷
	const Vector3 randPos = { 15, 15, 1 };
	pos.x += (float)((rand() % (int)randPos.x) - randPos.x / 2);
	pos.y += (float)((rand() % (int)randPos.y) - randPos.y / 2);
	pos.z += (float)((rand() % (int)randPos.z));

	//�������o�p�p�[�e�B�N������
	const float size = 0.75f;
	ParticleEmitter::GetInstance()->Explosion(pos, size);
}

void GalaxyBody::DamageColorMode()
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

void GalaxyBody::AppaerAdvance()
{
	//��̈ʒu�̑O�i���Ă���
	const float advanceSpeed = 0.8f;
	position.z -= advanceSpeed;

	//��~���W�܂Ői��ł��Ȃ���Δ�����
	if (position.z > basePos.z) { return; }
	
	//���̃t�F�[�Y��
	appearPhase = AppearPhase::Wait;
}

void GalaxyBody::AppaerWait()
{
	//�ҋ@���鎞��
	const int waitTime = 50;
	//�^�C�}�[�X�V
	actionTimer++;

	//�^�C�}�[���w�肵�����Ԉȉ��Ȃ甲����
	if (actionTimer < waitTime) { return; }

	//�^�C�}�[�����������Ă���
	actionTimer = 0;
	//���̃t�F�[�Y��
	appearPhase = AppearPhase::Rotation;
}

void GalaxyBody::AppaerRotation()
{
	//�E�����ɂ���
	const float rotSpeed = 0.5f;
	rotation.y += rotSpeed;

	//�E�����S�Ɍ����Ă��Ȃ���Δ�����
	if (rotation.y <= 90) { return; }

	//�o��s���I��
	isAppear = false;
}

void GalaxyBody::AttackPartChangeWait()
{
	//�ҋ@���鎞��
	const int waitTime = 100;
	//�^�C�}�[�X�V
	actionTimer++;

	//�^�C�}�[���w�肵�����Ԉȉ��Ȃ甲����
	if (actionTimer < waitTime) { return; }

	//�^�C�}�[�����������Ă���
	actionTimer = 0;
	//���̃t�F�[�Y��
	attackPartChangePhase = AttackPartChangePhase::Rotation;
}

void GalaxyBody::AttackPartChangeRotation()
{
	//��]���x�����Z
	rotation.y += attackPartChangeRotSpeed;

	//��]��̊p�x�܂ōs���Ă��Ȃ���Δ�����
	if (isAttackPartChangeRotaRight && rotation.y <= attackPartChangeRota) { return; }		//�E��]
	else if (!isAttackPartChangeRotaRight && rotation.y >= attackPartChangeRota) { return; }//����]

	//�U������p�[�c�ύX�������]���I��
	isAttackPartChangeRota = false;
}

void GalaxyBody::Sway()
{
	//�����ő�̑���
	const float rotXMaxSpeed = 0.02f;
	//�����܂�Ԃ�
	const float swayLimitBasePosDistance = 10;
	//�����̉����x
	const float swayXSpeedAccel = 0.001f;
	//��ړ�
	if (isSwayUp) {
		//���x���ő�łȂ��Ƃ�
		if (!isSwaySpeedMax) {
			//���x�ɉ����x�����Z����
			swayVel.y += swayXSpeedAccel;

			//�������ő�ɂȂ�����t���O�𗧂Ă�
			if (swayVel.y >= rotXMaxSpeed) {
				isSwaySpeedMax = true;
			}
		}
		//�����̍��W���܂�Ԃ��܂ŗ�����
		if (position.y >= basePos.y + swayLimitBasePosDistance) {
			//���x�ɉ����x�����Z���Ă���
			swayVel.y -= swayXSpeedAccel;

			//���x��0�ɂȂ�����
			if (swayVel.y <= 0) {
				//���ړ��ɕύX
				isSwayUp = false;
				//���x�ő�t���O�����낵�Ă���
				isSwaySpeedMax = false;
			}
		}
	}
	//���ړ�
	else {
		//���x���ő�łȂ��Ƃ�
		if (!isSwaySpeedMax) {
			//���x�ɉ����x�����Z����
			swayVel.y -= swayXSpeedAccel;

			//���x���ő�ɂȂ�����t���O�𗧂Ă�
			if (swayVel.y <= -rotXMaxSpeed) {
				isSwaySpeedMax = true;
			}
		}
		//�����̑��x���܂�Ԃ��܂ŗ�����
		if (position.y <= basePos.y - swayLimitBasePosDistance) {
			//���x�ɉ����x�����Z���Ă���
			swayVel.y += swayXSpeedAccel;

			//���x��0�ɂȂ�����
			if (swayVel.y >= 0) {
				//��ړ��ɕύX
				isSwayUp = true;
				//���x�ő�t���O�����낵�Ă���
				isSwaySpeedMax = false;
			}
		}
	}

	//Y���W�ɑ��x�����Z���Ă���炳����
	position += swayVel;
}
