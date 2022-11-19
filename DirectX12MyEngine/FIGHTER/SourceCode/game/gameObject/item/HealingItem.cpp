#include "HealingItem.h"
#include "Player.h"
#include "Easing.h"

Player* HealingItem::player = nullptr;

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
