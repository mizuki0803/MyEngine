#include "SortiePlayer.h"
#include "ParticleEmitter.h"
#include "GamePostEffect.h"

void (SortiePlayer::* SortiePlayer::sortieActionFuncTable[])() = {
	&SortiePlayer::SortieAdvance,
	&SortiePlayer::SortieBoost,
};

const float SortiePlayer::advanceSpeed = 1.3f;

SortiePlayer* SortiePlayer::Create(ObjModel* model, const Vector3& startPosition)
{
	//�o���V�[���p���@�̃C���X�^���X�𐶐�
	SortiePlayer* sortiePlayer = new SortiePlayer();
	if (sortiePlayer == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	sortiePlayer->model = model;

	// ������
	if (!sortiePlayer->Initialize()) {
		delete sortiePlayer;
		assert(0);
		return nullptr;
	}

	//�������W���Z�b�g
	sortiePlayer->position = startPosition;
	//���������Ă���
	const float size = 1.5f;
	sortiePlayer->scale = { size, size, size };

	return sortiePlayer;
}

void SortiePlayer::Update()
{
	//�s��
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieActionPhase)])();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();

	//���@�̃W�F�b�g���ˉ��o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->PlayerJet(matWorld, particleJetSizePhaseNum);
}

void SortiePlayer::BoostStart()
{
	//�u�[�X�g��Ԃɂ���
	sortieActionPhase = SortieActionPhase::Boost;

	//�p�[�e�B�N���W�F�b�g��傫������(�����p�ɂ���)
	const int32_t particleSizePhase = 1;
	particleJetSizePhaseNum = particleSizePhase;

	//�|�X�g�G�t�F�N�g�̃u���[��������
	GamePostEffect::GetPostEffect()->SetRadialBlur(true);
	const float blurStrength = 0.2f;
	GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
}

void SortiePlayer::SortieAdvance()
{
	//�O�i������
	position.z += advanceSpeed;
}

void SortiePlayer::SortieBoost()
{
	//�u�[�X�g������(�ʏ�X�s�[�h��2�{)
	const float boostSpeed = advanceSpeed * 2;
	position.z += boostSpeed;

	//�^�C�}�[�X�V
	boostTimer++;

	//��莞�Ԍo������o���s�����I��������
	const int endTime = 15;
	if (boostTimer >= endTime) {
		isSortieEnd = true;
	}
}
