#include "Stage01SortiePlayer.h"
#include "ParticleEmitter.h"
#include "GamePostEffect.h"

void (Stage01SortiePlayer::* Stage01SortiePlayer::sortieActionFuncTable[])() = {
	&Stage01SortiePlayer::SortieAdvance,
	&Stage01SortiePlayer::SortieBoost,
};

const float Stage01SortiePlayer::advanceSpeed = 1.3f;

Stage01SortiePlayer* Stage01SortiePlayer::Create(ObjModel* model, const Vector3& startPosition)
{
	//�X�e�[�W01�o���V�[���p���@�̃C���X�^���X�𐶐�
	Stage01SortiePlayer* sortiePlayer = new Stage01SortiePlayer();
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

void Stage01SortiePlayer::Update()
{
	//�s��
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieActionPhase)])();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();

	//�W�F�b�g���ˍ��W���X�V
	UpdateJetPos();
	//���@�̃W�F�b�g���ˉ��o�p�p�[�e�B�N������
	std::function<Vector3()> getTargetPos = std::bind(&Stage01SortiePlayer::GetJetPos, this);
	ParticleEmitter::GetInstance()->PlayerJet(getTargetPos, matWorld, particleJetSizePhaseNum);
}

void Stage01SortiePlayer::BoostStart()
{
	//�u�[�X�g��Ԃɂ���
	sortieActionPhase = Stage01SortieActionPhase::Boost;

	//�p�[�e�B�N���W�F�b�g��傫������(�����p�ɂ���)
	const int32_t particleSizePhase = 1;
	particleJetSizePhaseNum = particleSizePhase;

	//�|�X�g�G�t�F�N�g�̃u���[��������
	GamePostEffect::GetPostEffect()->SetRadialBlur(true);
	const float blurStrength = 0.8f;
	GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
}

void Stage01SortiePlayer::UpdateJetPos()
{
	//���@�̒��S���W����̋���
	const Vector3 distancePos = { 0, -0.25f, -1.2f };

	//�W�F�b�g���ˍ��W���擾
	jetPos = Vector3::LocalTranslation(distancePos, matWorld);
}

void Stage01SortiePlayer::SortieAdvance()
{
	//�O�i������
	position.z += advanceSpeed;
}

void Stage01SortiePlayer::SortieBoost()
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
