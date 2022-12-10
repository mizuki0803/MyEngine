#include "TitlePlayer.h"
#include "ParticleEmitter.h"
#include "Easing.h"
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
	const float blurStrengthMax = 0.6f; //�u���[�ő�̍L���鋭��
	if (!GamePostEffect::GetPostEffect()->GetIsRadialBlur()) {
		GamePostEffect::GetPostEffect()->SetRadialBlur(true);

		//�J�n���̃u���[�̋������Z�b�g
		GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrengthMax);
	}
	else {
		//���񂾂�u���[���キ���鎞��
		const int blurChangeTime = 300;
		//���Ɏキ�����甲����
		if ((sortieStartTimer - sortieStartTime) > blurChangeTime) { return; }

		//�u���[�ŏ��̍L���鋭��
		const float blurStrengthMin = 0.05f;

		//�C�[�W���O�ŋ�����ύX
		const float time = (float)(sortieStartTimer - sortieStartTime) / blurChangeTime;
		const float blurStrength = Easing::OutQuad(blurStrengthMax, blurStrengthMin, time);
		GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
	}
}
