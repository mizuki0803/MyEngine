#include "StageSelectPlayer.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "GamePostEffect.h"

void (StageSelectPlayer::* StageSelectPlayer::actionFuncTable[])() = {
	&StageSelectPlayer::GooutPlanet,
	&StageSelectPlayer::StageSelectMove,
	&StageSelectPlayer::EnterPlanet,
};

void (StageSelectPlayer::* StageSelectPlayer::enterPlanetActionFuncTable[])() = {
	&StageSelectPlayer::EnterPlanetRotate,
	&StageSelectPlayer::EnterPlanetStay,
	&StageSelectPlayer::EnterPlanetBoost,
};

StageSelectPlayer* StageSelectPlayer::Create(ObjModel* model, const Vector3& startPosition)
{
	//�X�e�[�W�I���V�[���p���@�̃C���X�^���X�𐶐�
	StageSelectPlayer* stageSelectPlayer = new StageSelectPlayer();
	if (stageSelectPlayer == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	stageSelectPlayer->model = model;

	// ������
	if (!stageSelectPlayer->Initialize()) {
		delete stageSelectPlayer;
		assert(0);
		return nullptr;
	}

	//�������W���Z�b�g
	stageSelectPlayer->position = startPosition;

	//�傫�����Z�b�g
	const float size = 1.5f;
	stageSelectPlayer->scale = { size ,size ,size };

	return stageSelectPlayer;
}

void StageSelectPlayer::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(actionPhase)])();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();

	//�W�F�b�g���ˍ��W���X�V
	UpdateJetPos();
	//���@�̃W�F�b�g���ˉ��o�p�p�[�e�B�N������
	std::function<Vector3()> getTargetPos = std::bind(&StageSelectPlayer::GetJetPos, this);
	ParticleEmitter::GetInstance()->PlayerJet(getTargetPos, matWorld, particleJetSizePhaseNum);
}

void StageSelectPlayer::GooutPlanetStart(const Vector3& targetPos)
{
	//�ړ��O���W���Z�b�g
	moveBeforePos = position;
	//�ړ�����W���Z�b�g
	const Vector3 distance = { 0, 5, 0 };
	const Vector3 afterPos = targetPos + distance;
	moveAfterPos = afterPos;

	//�f������o�Ă����Ԃɂ���
	actionPhase = ActionPhase::GooutPlanet;

	//�s���^�C�}�[��������
	actionTimer = 0;
}

void StageSelectPlayer::StageSelectMoveStart(const Vector3& targetPos, bool isStageSelectMoveRight)
{
	//�ړ��O���W���Z�b�g
	moveBeforePos = position;
	//�ړ�����W���Z�b�g
	const Vector3 distance = { 0, 5, 0 };
	const Vector3 afterPos = targetPos + distance;
	moveAfterPos = afterPos;

	//�ړ��������������
	rotation = Vector3::BetweenPointRotate(moveAfterPos, moveBeforePos);
	//�ύX�O�̊p�x���L�^
	changeBeforeRota = rotation;

	//�ړ����������Ȃ�p�x���C��
	if (!isStageSelectMoveRight) { changeBeforeRota.y += 360; }

	//�X�e�[�W�I���ňړ����ɂ���
	isStageSelectMove = true;

	//�s���^�C�}�[��������
	actionTimer = 0;
}

void StageSelectPlayer::EnterPlanetStart(const Vector3& selectPlanetPos)
{
	//�ړ��O���W���Z�b�g
	moveBeforePos = position;
	//�ړ�����W���Z�b�g
	moveAfterPos = selectPlanetPos;
	//�f���̎�O�ɐݒ肵�Ă���
	const Vector3 distance = { 0, 6.0f, -27.0f };
	moveAfterPos += distance;

	//�ύX�O�̊p�x���Z�b�g
	changeBeforeRota = rotation;

	//�ύX��̊p�x���Z�b�g
	changeAfterRota = Vector3::BetweenPointRotate(moveAfterPos, moveBeforePos);

	//�f���ɓ����Ԃɂ���
	actionPhase = ActionPhase::EnterPlanet;

	//�s���^�C�}�[��������
	actionTimer = 0;
}

void StageSelectPlayer::UpdateJetPos()
{
	//���@�̒��S���W����̋���
	const Vector3 distancePos = { 0, -0.25f, -1.2f };

	//�W�F�b�g���ˍ��W���擾
	jetPos = Vector3::LocalTranslation(distancePos, matWorld);
}

void StageSelectPlayer::GooutPlanet()
{
	//�f������o�Ă���̂ɂ����鎞��
	const float gooutTime = 210;
	//�^�C�}�[�X�V
	actionTimer++;
	const float time = actionTimer / gooutTime;

	//�ړ��������������
	rotation = Vector3::BetweenPointRotate(moveAfterPos, moveBeforePos);

	//�C�[�W���O�œ�����
	position.x = Easing::OutQuint(moveBeforePos.x, moveAfterPos.x, time);
	position.y = Easing::OutQuint(moveBeforePos.y, moveAfterPos.y, time);
	position.z = Easing::OutQuint(moveBeforePos.z, moveAfterPos.z, time);

	//�傫����ς���
	const float beforeSize = 0.1f;
	const float afterSize = 1.5f;
	scale.x = Easing::InQuint(beforeSize, afterSize, time);
	scale.y = Easing::InQuint(beforeSize, afterSize, time);
	scale.z = Easing::InQuint(beforeSize, afterSize, time);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (actionTimer >= gooutTime) {
		//���̃t�F�[�Y��
		actionPhase = ActionPhase::StageSelect;

		//�f������o�Ă���s�����I����
		isGooutPlanetEnd = true;
	}
}

void StageSelectPlayer::StageSelectMove()
{
	//�X�e�[�W�I���ňړ�����ԂłȂ���Δ�����
	if (!isStageSelectMove) { return; }

	//�ړ��ɂ����鎞��
	const float moveTime = 40;
	//�^�C�}�[�X�V
	actionTimer++;
	const float time = actionTimer / moveTime;

	//�C�[�W���O�œ�����
	position.x = Easing::OutCubic(moveBeforePos.x, moveAfterPos.x, time);
	position.y = Easing::OutCubic(moveBeforePos.y, moveAfterPos.y, time);
	position.z = Easing::OutCubic(moveBeforePos.z, moveAfterPos.z, time);

	//�c��̎w�肵�����Ԃ���p�x���C������
	const float rotChangeTime = 10;
	if (actionTimer >= moveTime - rotChangeTime) {
		const float rotTime = (actionTimer - (moveTime - rotChangeTime)) / rotChangeTime;
		//�C�[�W���O�œ�����
		const float changeAfterRota = 180;
		rotation.y = Easing::OutCubic(changeBeforeRota.y, changeAfterRota, rotTime);
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (actionTimer >= moveTime) {
		//�X�e�[�W�I���ňړ�����Ԃ�����
		isStageSelectMove = false;
	}
}

void StageSelectPlayer::EnterPlanet()
{
	//�f���ɓ���s��
	(this->*enterPlanetActionFuncTable[static_cast<size_t>(enterPlanetActionPhase)])();
}

void StageSelectPlayer::EnterPlanetRotate()
{
	//��]�ɂ����鎞��
	const float rotTime = 180;
	//�^�C�}�[�X�V
	actionTimer++;
	const float time = actionTimer / rotTime;

	//�C�[�W���O�ŉ�]������
	rotation.x = Easing::OutQuint(changeBeforeRota.x, changeAfterRota.x, time);
	rotation.y = Easing::OutQuint(changeBeforeRota.y, changeAfterRota.y, time);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (actionTimer >= rotTime) {
		//���̃t�F�[�Y��
		enterPlanetActionPhase = EnterPlanetActionPhase::Stay;

		//�^�C�}�[��������
		actionTimer = 0;
	}
}

void StageSelectPlayer::EnterPlanetStay()
{
	//�ҋ@���鎞��
	const float stayTime = 10;
	//�^�C�}�[�X�V
	actionTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (actionTimer >= stayTime) {
		//���̃t�F�[�Y��
		enterPlanetActionPhase = EnterPlanetActionPhase::Boost;

		//�^�C�}�[��������
		actionTimer = 0;

		//�p�[�e�B�N���W�F�b�g��傫������(�����p�ɂ���)
		const int32_t particleSizePhase = 1;
		particleJetSizePhaseNum = particleSizePhase;

		//�|�X�g�G�t�F�N�g�Ƀ��W�A���u���[��������
		if (!GamePostEffect::GetPostEffect()->GetIsRadialBlur()) {
			GamePostEffect::GetPostEffect()->SetRadialBlur(true);

			//�u���[�̋������Z�b�g
			const float blurStrength = 0.4f;
			GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
		}
	}
}

void StageSelectPlayer::EnterPlanetBoost()
{
	//�f������̂ɂ����鎞��
	const float enterTime = 100;
	//�^�C�}�[�X�V
	actionTimer++;
	float time = actionTimer / enterTime;
	time = min(time, 1);

	//�ړ��������������
	rotation = Vector3::BetweenPointRotate(moveAfterPos, moveBeforePos);

	//�C�[�W���O�œ�����
	position.x = Easing::OutQuint(moveBeforePos.x, moveAfterPos.x, time);
	position.y = Easing::OutQuint(moveBeforePos.y, moveAfterPos.y, time);
	position.z = Easing::OutQuint(moveBeforePos.z, moveAfterPos.z, time);

	//�傫����ς���
	const float beforeSize = 1.5f;
	const float afterSize = 0.2f;
	scale.x = Easing::OutQuint(beforeSize, afterSize, time);
	scale.y = Easing::OutQuint(beforeSize, afterSize, time);
	scale.z = Easing::OutQuint(beforeSize, afterSize, time);

	//�^�C�}�[���f���ɓ���̂��I���鎞�ԂɂȂ�����
	const int endTime = 120;
	if (actionTimer >= endTime) {
		//�X�e�[�W�I���ōs���S�Ă̍s�����I����
		isStageSelectModeEnd = true;
	}
}
