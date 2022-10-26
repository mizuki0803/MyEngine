#include "SortiePlayer.h"
#include "ParticleEmitter.h"

void (SortiePlayer::* SortiePlayer::sortieActionFuncTable[])() = {
	&SortiePlayer::SortieAdvance,
	&SortiePlayer::SortieBoost,
};

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
	ParticleEmitter::GetInstance()->PlayerJet(matWorld);
}

void SortiePlayer::BoostStart()
{
	//�u�[�X�g��Ԃɂ���
	sortieActionPhase = SortieActionPhase::Boost;
}

void SortiePlayer::SortieAdvance()
{
	//�O�i������
	const float advanceSpeed = 1.0f;
	position.z += advanceSpeed;
}

void SortiePlayer::SortieBoost()
{
	//�u�[�X�g������
	const float advanceSpeed = 2.0f;
	position.z += advanceSpeed;

	//�^�C�}�[�X�V
	boostTimer++;

	//��莞�Ԍo������o���s�����I��������
	const int endTime = 30;
	if (boostTimer >= endTime) {
		isSortieEnd = true;
	}
}
