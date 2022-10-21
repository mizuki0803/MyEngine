#include "HealingItem.h"
#include "Player.h"

Player* HealingItem::player = nullptr;

HealingItem* HealingItem::Create(ObjModel* model, const Vector3& position)
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

	//�f�o�b�O�p
	healingItem->color = { 1,0,0,1 };
	healingItem->scale = { 0.5f, 0.5f, 0.5f };

	return healingItem;
}

void HealingItem::Update()
{
	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();
}

void HealingItem::OnCollision()
{
    //���S������
    isDead = true;
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

void HealingItem::FrontOfScreenDelete()
{
	//���W�����@����O(��ʊO��O)�܂ōs������폜
	const float flontOfScreenDiffence = 100;
	const float deletePos = player->GetWorldPos().z - flontOfScreenDiffence;
	const Vector3 worldPos = GetWorldPos();

	if (worldPos.z <= deletePos) {
		isDead = true;
	}
}
