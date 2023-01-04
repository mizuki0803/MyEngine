#include "TitlePlayer.h"
#include "ParticleEmitter.h"
#include "Easing.h"
#include "GamePostEffect.h"

void (TitlePlayer::* TitlePlayer::sortieActionFuncTable[])() = {
	&TitlePlayer::SortieStay,
	&TitlePlayer::SortieBoost,
};

TitlePlayer* TitlePlayer::Create(ObjModel* model, const Vector3& startPosition)
{
	//�^�C�g���V�[���p���@�̃C���X�^���X�𐶐�
	TitlePlayer* titlePlayer = new TitlePlayer();
	if (titlePlayer == nullptr) {
		return nullptr;
	}

	// ������
	if (!titlePlayer->Initialize(model, startPosition)) {
		delete titlePlayer;
		assert(0);
		return nullptr;
	}

	return titlePlayer;
}

bool TitlePlayer::Initialize(ObjModel* model, const Vector3& startPosition)
{
	//�I�u�W�F�N�g�̏�����
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//���f�����Z�b�g
	assert(model);
	this->model = model;

	//���W���Z�b�g
	position = startPosition;

	//��s�@�_���o����
	vaporEffect.reset(new PlayerVaporEffect());

	return true;
}

void TitlePlayer::Update()
{
	//�o��
	if (isSortie) {
		Sortie();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();

	//�����̍��W�X�V
	UpdateWingPos();
}

void TitlePlayer::Sortie()
{
	//�o���s��
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieModePhase)])();

	//���@�̃W�F�b�g���ˉ��o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->PlayerJet(matWorld);
}

void TitlePlayer::SortieStay()
{
	//���˂���܂ł̎���
	const float sortieStartTime = 180;
	sortieTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (sortieTimer >= sortieStartTime) {
		//���̃t�F�[�Y��
		sortieModePhase = SortieModePhase::Boost;
		//�^�C�}�[��������
		sortieTimer = 0;

		//��s�@�_�̐������J�n����
		vaporEffect->VaporStart();
	}
}

void TitlePlayer::SortieBoost()
{
	//�^�C�}�[�X�V
	sortieTimer++;

	//��]���x
	const float rotSpeed = 0.2f;
	//x����]
	rotation.x -= rotSpeed;
	const float rotLimit = -45.0f;
	rotation.x = max(rotation.x, rotLimit);

	//���@���X���Ă���p�x�Ɉړ�������
	Vector3 moveSpeed = { 0, 5, 5 };
	moveSpeed.y = moveSpeed.y * (rotation.x / rotLimit);
	position += moveSpeed;

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
		if (sortieTimer > blurChangeTime) { return; }

		//�u���[�ŏ��̍L���鋭��
		const float blurStrengthMin = 0.05f;

		//�C�[�W���O�ŋ�����ύX
		const float time = (float)sortieTimer / blurChangeTime;
		const float blurStrength = Easing::OutQuad(blurStrengthMax, blurStrengthMin, time);
		GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
	}
}

void TitlePlayer::UpdateWingPos()
{
	//���@�̒��S���W����̋���
	const Vector3 leftDistancePos = { -2.0f, -0.2f, -0.9f };
	const Vector3 rightDistancePos = { 2.0f, -0.2f, -0.9f };
	//�������W���擾
	const Vector3 leftWingPos = LocalTranslation(leftDistancePos, matWorld);
	//�E�����W���擾
	const Vector3 rightWingPos = LocalTranslation(rightDistancePos, matWorld);

	vaporEffect->Update(leftWingPos, rightWingPos);
}
