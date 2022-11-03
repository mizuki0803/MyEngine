#include "StageSelectPlayer.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (StageSelectPlayer::* StageSelectPlayer::actionFuncTable[])() = {
	&StageSelectPlayer::GooutPlanet,
	&StageSelectPlayer::StageSelectMove,
	&StageSelectPlayer::EnterPlanet,
};

void (StageSelectPlayer::* StageSelectPlayer::enterPlanetActionFuncTable[])() = {
	&StageSelectPlayer::EnterPlanetRotate,
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

	//�p�[�e�B�N�����o
	ParticleEmitter::GetInstance()->PlayerJet(matWorld);
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
	const Vector3 moveVec = moveAfterPos - moveBeforePos;
	rotation.y = XMConvertToDegrees(std::atan2(moveVec.x, moveVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 moveVecZ = MatrixTransformDirection(moveVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-moveVecZ.y, moveVecZ.z));
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
	const Vector3 moveVec = moveAfterPos - moveBeforePos;
	changeAfterRota.y = XMConvertToDegrees(std::atan2(moveVec.x, moveVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-changeAfterRota.y));
	Vector3 moveVecZ = MatrixTransformDirection(moveVec, matRot);
	changeAfterRota.x = XMConvertToDegrees(std::atan2(-moveVecZ.y, moveVecZ.z));

	//�f���ɓ����Ԃɂ���
	actionPhase = ActionPhase::EnterPlanet;

	//�s���^�C�}�[��������
	actionTimer = 0;
}

void StageSelectPlayer::GooutPlanet()
{
	//�f������o�Ă���̂ɂ����鎞��
	const float gooutTime = 300;
	//�^�C�}�[�X�V
	actionTimer++;
	const float time = actionTimer / gooutTime;

	//�ړ��������������
	const Vector3 moveVec = moveAfterPos - moveBeforePos;
	rotation.y = XMConvertToDegrees(std::atan2(moveVec.x, moveVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 moveVecZ = MatrixTransformDirection(moveVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-moveVecZ.y, moveVecZ.z));

	//�C�[�W���O�œ�����
	position.x = Easing::OutQuint(moveBeforePos.x, moveAfterPos.x, time);
	position.y = Easing::OutQuint(moveBeforePos.y, moveAfterPos.y, time);
	position.z = Easing::OutQuint(moveBeforePos.z, moveAfterPos.z, time);

	//�傫����ς���
	scale.x = Easing::InQuint(0.1f, 1.5f, time);
	scale.y = Easing::InQuint(0.1f, 1.5f, time);
	scale.z = Easing::InQuint(0.1f, 1.5f, time);

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
	const float moveTime = 50;
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
		rotation.y = Easing::OutCubic(changeBeforeRota.y, 180, rotTime);
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
		enterPlanetActionPhase = EnterPlanetActionPhase::Boost;

		//�^�C�}�[��������
		actionTimer = 0;
	}
}

void StageSelectPlayer::EnterPlanetBoost()
{
	//�f������̂ɂ����鎞��
	const float enterTime = 300;
	//�^�C�}�[�X�V
	actionTimer++;
	const float time = actionTimer / enterTime;

	//�ړ��������������
	const Vector3 moveVec = moveAfterPos - moveBeforePos;
	rotation.y = XMConvertToDegrees(std::atan2(moveVec.x, moveVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 moveVecZ = MatrixTransformDirection(moveVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-moveVecZ.y, moveVecZ.z));

	//�C�[�W���O�œ�����
	position.x = Easing::OutQuint(moveBeforePos.x, moveAfterPos.x, time);
	position.y = Easing::OutQuint(moveBeforePos.y, moveAfterPos.y, time);
	position.z = Easing::OutQuint(moveBeforePos.z, moveAfterPos.z, time);

	//�傫����ς���
	scale.x = Easing::OutQuint(1.5f, 0.2f, time);
	scale.y = Easing::OutQuint(1.5f, 0.2f, time);
	scale.z = Easing::OutQuint(1.5f, 0.2f, time);

	//�^�C�}�[���f���ɓ���̂��I���鎞�ԂɂȂ�����
	const int endTime = 240;
	if (actionTimer >= endTime) {
		//�X�e�[�W�I���ōs���S�Ă̍s�����I����
		isStageSelectModeEnd = true;
	}
}
