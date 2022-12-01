#include "TitlePlayer.h"
#include "ParticleEmitter.h"
#include "GamePostEffect.h"

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
	//���˂���܂ł̎���
	const float sortieStartTime = 180;
	sortieStartTimer++;

	//���@�̃W�F�b�g���ˉ��o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->PlayerJet(matWorld);

	if (sortieStartTimer <= sortieStartTime) { return; }

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

	//�|�X�g�G�t�F�N�g�Ƀ��W�A���u���[��������
	if (!GamePostEffect::GetPostEffect()->GetIsRadialBlur()) {
		GamePostEffect::GetPostEffect()->SetRadialBlur(true);

		//�J�n���̃u���[�̋������Z�b�g
		const float blurStrength = 0.5f;
		GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
	}
	else {
		//���񂾂�u���[���キ����
		const int blurChangeTime = 10; //10�t���[�����キ����
		if (sortieStartTimer % 10 != 0) { return; }
		float blurStrength = GamePostEffect::GetPostEffect()->GetRadialBlurStrength();
		blurStrength -= 0.02f;
		//�u���[�̍L���鋭�����w�肵���l�������Ȃ��悤�ɂ���
		const float blurStrengthMin = 0.1f;
		if (blurStrength <= blurStrengthMin) { blurStrength = blurStrengthMin; }
		GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
	}
}
