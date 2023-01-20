#include "Stage02SortiePlayer.h"
#include "ParticleEmitter.h"
#include "GamePostEffect.h"
#include "BaseStageScene.h"
#include "StraightBullet.h"

void (Stage02SortiePlayer::* Stage02SortiePlayer::sortieActionFuncTable[])() = {
	&Stage02SortiePlayer::SortieAdvance,
	&Stage02SortiePlayer::SortieBoost,
};

BaseStageScene* Stage02SortiePlayer::stageScene = nullptr;
ObjModel* Stage02SortiePlayer::bulletModel = nullptr;
const float Stage02SortiePlayer::advanceSpeed = 1.0f;

Stage02SortiePlayer* Stage02SortiePlayer::Create(ObjModel* model, const Vector3& startPosition)
{
	//�X�e�[�W01�o���V�[���p���@�̃C���X�^���X�𐶐�
	Stage02SortiePlayer* sortiePlayer = new Stage02SortiePlayer();
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

void Stage02SortiePlayer::Update()
{
	//���ˍ��W�X�V
	UpdateBulletShotPos();

	//�e����
	Fire();

	//�s��
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieActionPhase)])();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();

	//�W�F�b�g���ˍ��W���X�V
	UpdateJetPos();
	//���@�̃W�F�b�g���ˉ��o�p�p�[�e�B�N������
	std::function<Vector3()> getTargetPos = std::bind(&Stage02SortiePlayer::GetJetPos, this);
	ParticleEmitter::GetInstance()->PlayerJet(getTargetPos, matWorld, particleJetSizePhaseNum);
}

void Stage02SortiePlayer::BoostStart()
{
	//�u�[�X�g��Ԃɂ���
	sortieActionPhase = Stage02SortieActionPhase::Boost;

	//�p�[�e�B�N���W�F�b�g��傫������(�����p�ɂ���)
	const int32_t particleSizePhase = 1;
	particleJetSizePhaseNum = particleSizePhase;

	//�|�X�g�G�t�F�N�g�̃u���[��������
	GamePostEffect::GetPostEffect()->SetRadialBlur(true);
	const float blurStrength = 0.8f;
	GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
}

void Stage02SortiePlayer::UpdateJetPos()
{
	//���@�̒��S���W����̋���
	const Vector3 distancePos = { 0, -0.25f, -1.2f };

	//�W�F�b�g���ˍ��W���擾
	jetPos = Vector3::LocalTranslation(distancePos, matWorld);
}

void Stage02SortiePlayer::UpdateBulletShotPos()
{
	//���@�̒��S���W����̋���
	const Vector3 distancePos = { 0, -0.3f, 5.0f };

	//�e���ˍ��W���擾
	bulletShotPos = Vector3::LocalTranslation(distancePos, matWorld);
}

void Stage02SortiePlayer::ShotStraightBullet()
{
	//�e�̑��x��ݒ�
	const float bulletSpeed = 4;
	//���@�̌����Ă�������ɔ�΂�
	Vector3 velocity = { 0, 0, bulletSpeed };
	velocity = MatrixTransformDirection(velocity, matWorld);

	//���i�e�𐶐�
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(StraightBullet::Create(bulletModel, bulletShotPos, velocity));
	stageScene->AddPlayerBullet(std::move(newBullet));

	//�V���b�g���o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->Shot(bulletShotPos);
}

void Stage02SortiePlayer::Fire()
{
	//�w�肵�����W�ȉ��Ȃ甲����
	const float fireStartPos = 270;
	if (position.z < fireStartPos) { return; }
	//����3�񔭎˂��Ă����甲����
	if (fireCount > 3) { return; }

	//�e���ˊԊu
	const int fireInterval = 8;
	//�^�C�}�[�X�V
	fireTimer++;
	if (fireTimer <= fireInterval) { return; }

	//�e����
	ShotStraightBullet();
	//���ˉ񐔍X�V
	fireCount++;
	//�^�C�}�[������
	fireTimer = 0;
}

void Stage02SortiePlayer::SortieAdvance()
{
	//�O�i������
	position.z += advanceSpeed;
}

void Stage02SortiePlayer::SortieBoost()
{
	//�u�[�X�g������(�ʏ�X�s�[�h��3�{)
	const float boostSpeed = advanceSpeed * 3;
	position.z += boostSpeed;

	//�^�C�}�[�X�V
	boostTimer++;

	//��莞�Ԍo������o���s�����I��������
	const int endTime = 50;
	if (boostTimer >= endTime) {
		isSortieEnd = true;
	}
}
