#pragma once
#include <DirectXMath.h>
#include "Camera.h"

//�X�e�[�W�I���V�[���p���@�N���X��O���錾
class StageSelectPlayer;

/// <summary>
/// �o���V�[���p�J����
/// </summary>
class StageSelectCamera : public Camera
{
public:
	//�s���t�F�[�Y
	enum class ActionPhase {
		GooutPlayerLook,	//�f������o�Ă��鎩�@������
		StageSelectStart,	//�X�e�[�W�I���J�n
		StageSelect,		//�X�e�[�W�I��
		StageDecisionCheck,	//�X�e�[�W����m�F
		ReturnSelect,		//�X�e�[�W�I���ɖ߂�
	};

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �f������o�Ă��鎩�@������̓����J�n
	/// </summary>
	void GooutPlayerLookActionStart(const Vector3& gooutPlanetPos, const Vector3& selectFieldPos);

	/// <summary>
	/// �X�e�[�W�I���J�n�̓����J�n
	/// </summary>
	void StageSelectStartActionStart();

	/// <summary>
	/// �X�e�[�W����m�F�̓����J�n
	/// </summary>
	void StageDecisionCheckActionStart();

	/// <summary>
	/// �X�e�[�W�I���ɖ߂铮���J�n
	/// </summary>
	void ReturnSelectActionStart();

	//getter
	ActionPhase GetActionPhase() { return phase; }

	//setter
	void SetStageSelectPlayer(StageSelectPlayer* player) { this->player = player; }

private: //�����o�֐�
	/// <summary>
	/// �f������o�Ă��鎩�@������̓���
	/// </summary>
	void GooutPlayerLookAction();

	/// <summary>
	/// �X�e�[�W�I���J�n�̓���
	/// </summary>
	void StageSelectStartAction();

	/// <summary>
	/// �X�e�[�W�I����Ԃ̓���
	/// </summary>
	void StageSelectAction();

	/// <summary>
	/// �X�e�[�W����m�F�̓���
	/// </summary>
	void StageDecisionCheckAction();

	/// <summary>
	/// �X�e�[�W�I���ɖ߂��Ԃ̓���
	/// </summary>
	void ReturnSelectAction();

private: //�ÓI�����o�ϐ�
	//�s���J��
	static void (StageSelectCamera::* actionFuncTable[])();
	//�f������o�Ă��鎩�@�������Ԃ̃X�e�[�W�I���t�B�[���h�ƃJ�����̋���
	static const Vector3 gooutPlayerLookSelectFieldDistance;
	//�X�e�[�W�I����Ԃŕ������鎩�@�ƃJ�����̋���
	static const Vector3 stageSelectPlayerDistance;
	//�X�e�[�W����m�F��Ԃŕ������鎩�@�ƃJ�����̋���
	static const Vector3 stageDecisionCheckPlayerDistance;

private: //�����o�ϐ�
	//�v���C���[���@
	StageSelectPlayer* player = nullptr;
	//�J��������
	ActionPhase phase = ActionPhase::StageSelect;
	//�J�����s���p�^�C�}�[
	int32_t cameraActionTimer = 0;
	//���@���o�Ă���f���̍��W
	Vector3 gooutPlanetPos;
	//�ړ��O���_
	Vector3 moveBeforeEye;
	//�ړ��O�����_
	Vector3 moveBeforeTarget;
};