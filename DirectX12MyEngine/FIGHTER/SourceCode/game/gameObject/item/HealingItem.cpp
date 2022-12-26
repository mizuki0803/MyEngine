#include "HealingItem.h"
#include "BasePlayer.h"
#include "Easing.h"
#include "ParticleEmitter.h"

BasePlayer* HealingItem::player = nullptr;
const float HealingItem::shineEffectCreateSize = 1.5f;

HealingItem* HealingItem::Create(ObjModel* model, const Vector3& position, const float size)
{
	//�񕜃A�C�e���̃C���X�^���X�𐶐�
	HealingItem* healingItem = new HealingItem();
	if (healingItem == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	healingItem->model = model;

	// ������
	if (!healingItem->Initialize()) {
		delete healingItem;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	healingItem->position = position;

	//�傫�����Z�b�g
	healingItem->scale = { size, size, size };

	//�L���L�����o�T�C�Y���Z�b�g
	healingItem->shineEffectSize = shineEffectCreateSize;

	return healingItem;
}

void HealingItem::Update()
{
	//���邮���]
	Round();

	//�ڐG��̓���
	if (isTouched) {
		TouchedAction();
	}

	//�L���L�����o
	ShineEffect();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();
}

void HealingItem::OnCollision()
{
	//�ڐG����
	isTouched = true;

	//��]�̑������A�b�v
	const float deadRotSpeed = 15.0f;
	rotSpeed = deadRotSpeed;

	//�L���L�����o�T�C�Y��傫��
	shineEffectSize = shineEffectCreateSize;
	//�L���L�����o�����Ԋu���Z�b�g
	shineEffectInterval = shineEffectCreateInterval;
	//�L���L�����o���������Z�b�g
	shineEffectNum = 5;
}

Vector3 HealingItem::GetWorldPos()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���s�ړ��������擾
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void HealingItem::Round()
{
	//���邮���]
	rotation.y -= rotSpeed;
}

void HealingItem::FrontOfScreenDelete()
{
	//���W�����@����O(��ʊO��O)�܂ōs������폜
	const float flontOfScreenDiffence = 100;
	const float deletePos = player->GetWorldPos().z - flontOfScreenDiffence;
	const Vector3 worldPos = GetWorldPos();

	if (worldPos.z <= deletePos) {
		isDelete = true;
	}
}

void HealingItem::ShineEffect()
{
	//�ڐG���莞�Ԍo���Ă����甲����
	const int touchedAfterEffectCreateTime = 30;
	if (touchedTimer >= touchedAfterEffectCreateTime) { return; }

	//�L���L�����o�p�^�C�}�[�X�V
	shineEffectTimer++;

	//�傫���ύX
	ShineEffectSizeChange();
	//�����Ԋu�ύX
	ShineEffectIntervalChange();

	//�w�肵���Ԋu�ȊO�Ȃ甲����
	if (shineEffectTimer % shineEffectInterval != 0) { return; }

	//�L���L�����o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->ItemShine(position, shineEffectSize, shineEffectNum);

	//�������ύX
	shineEffectNum--;
	shineEffectNum = max(shineEffectNum, 1);
}

void HealingItem::ShineEffectSizeChange()
{
	//�ŏ��T�C�Y
	const float shineEffectSizeMin = 0.8f;
	//���ɍŏ��l�Ȃ甲����
	if (shineEffectSize <= shineEffectSizeMin) { return; }

	//1�t���[���ŏ���������l
	const float sizeChangeNum = 0.01f;
	shineEffectSize -= sizeChangeNum;
}

void HealingItem::ShineEffectIntervalChange()
{
	//�L���L�����o�����Ԋu�̍ő�l
	const int intarvalMax = 15;
	//���ɍő�l�Ȃ甲����
	if (shineEffectInterval >= intarvalMax) { return; }

	//�L���L�����o�����Ԋu��ύX���鎞��
	const int shineEffectIntervalChangeTime = 10;
	//�w�肵���Ԋu�ȊO�Ȃ甲����
	if (shineEffectTimer % shineEffectIntervalChangeTime != 0) { return; }

	//�����Ԋu��傫������
	shineEffectInterval++;
}

void HealingItem::TouchedAction()
{
	//�^�C�}�[�X�V
	const float deadTime = 70;
	const float moveTime = 30;
	touchedTimer++;

	//�ړ��C�[�W���O�Ɏg�p����ϐ�(0�`1���Z�o)
	float easeMoveTime = touchedTimer / moveTime;
	easeMoveTime = min(easeMoveTime, 1);
	//���@�̍��W�Ɉړ�����
	position = Easing::LerpVec3(position, player->GetWorldPos(), easeMoveTime);

	//�傫���ύX�C�[�W���O�Ɏg�p����ϐ�(0�`1���Z�o)
	float easeScaleTime = touchedTimer / deadTime;
	//�傫����ύX����
	const float startSize = 2.5f;
	scale.x = Easing::InQuint(startSize, 0, easeScaleTime);
	scale.y = Easing::InQuint(startSize, 0, easeScaleTime);
	scale.z = Easing::InQuint(startSize, 0, easeScaleTime);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (touchedTimer >= deadTime) {
		//�폜
		isDelete = true;
	}
}
