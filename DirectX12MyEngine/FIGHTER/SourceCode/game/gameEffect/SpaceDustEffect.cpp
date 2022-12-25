#include "SpaceDustEffect.h"

ObjModel* SpaceDustEffect::spaceDustModel = nullptr;
Player* SpaceDustEffect::player = nullptr;
GameCamera* SpaceDustEffect::gameCamera = nullptr;
const float SpaceDustEffect::spaceDustSize = 0.1f;
bool SpaceDustEffect::isScrollMode = false;

SpaceDustEffect* SpaceDustEffect::Create(const Vector3& position, const Vector3& rotation)
{
	//�F���o�G�t�F�N�g�̃C���X�^���X�𐶐�
	SpaceDustEffect* spaceDustEffect = new SpaceDustEffect();
	if (spaceDustEffect == nullptr) {
		return nullptr;
	}

	// ������
	if (!spaceDustEffect->Initialize()) {
		delete spaceDustEffect;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(spaceDustModel);
	spaceDustEffect->model = spaceDustModel;
	//���W���Z�b�g
	spaceDustEffect->position = position;
	//��]�p���Z�b�g
	spaceDustEffect->rotation = rotation;
	//�傫�����Z�b�g
	spaceDustEffect->scale = { spaceDustSize, spaceDustSize, spaceDustSize };

	return spaceDustEffect;
}

void SpaceDustEffect::Update()
{
	//���W���X�V
	Vector3 velocity = { 0, 0, -0.05f };
	//�X�N���[����ԂȂ�
	if (isScrollMode) {
		//���@�̈ړ����x��Ԃɂ���ĉF���o�G�t�F�N�g���J�����ňړ����Ă������x�œ�����
		float scrollSpeed = GameCamera::GetAdvanceSpeed();
		if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::HighSpeed) { scrollSpeed *= GameCamera::GetHighSpeedMagnification(); }
		else if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::SlowSpeed) { scrollSpeed *= GameCamera::GetSlowSpeedMagnification(); }

		velocity.z -= scrollSpeed;
	}
	position += velocity;

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();

	//��O�܂ōs������폜
	FrontOfScreenDelete();
}

void SpaceDustEffect::FrontOfScreenDelete()
{
	//���W�����@����O(��ʊO��O)�܂ōs������폜
	const float flontOfScreenDiffence = 10;
	const float deletePos = gameCamera->GetPosition().z - flontOfScreenDiffence;

	if (position.z <= deletePos) {
		isDelete = true;
	}
}
