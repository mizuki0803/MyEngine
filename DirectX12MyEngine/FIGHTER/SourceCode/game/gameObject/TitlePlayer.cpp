#include "TitlePlayer.h"
#include "ParticleEmitter.h"

TitlePlayer* TitlePlayer::Create(ObjModel* model, const Vector3& startPosition)
{
	//�^�C�g���V�[���p���@�̃C���X�^���X�𐶐�
	TitlePlayer* titlePlayer = new TitlePlayer();
	if (titlePlayer == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	titlePlayer->model = model;

	// ������
	if (!titlePlayer->Initialize()) {
		delete titlePlayer;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	titlePlayer->position = startPosition;

	return titlePlayer;
}

void TitlePlayer::Update()
{
	//�o��
	if (isSortie) {
		Sortie();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void TitlePlayer::Sortie()
{
	const float sortieStartTime = 180;
	if (sortieStartTimer <= sortieStartTime) {
		sortieStartTimer++;
	}
	else {
		//��]���x
		const float rotSpeed = 0.2f;
		//x����]
		rotation.x -= rotSpeed;
		const float rotLimit = -45.0f;
		rotation.x = max(rotation.x, rotLimit);

		//���@���X���Ă���p�x�Ɉړ�������
		Vector3 move = { 0, 0, 2.5f };
		const float moveSpeed = 2.5f;
		move.y = moveSpeed * (rotation.x / rotLimit);
		position += move;

	}

	//���@�̃W�F�b�g���ˉ��o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->PlayerJet(matWorld);
}
