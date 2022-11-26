#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �X�e�[�W�I���V�[���p���@
/// </summary>
class StageSelectPlayer : public ObjObject3d
{
public:
	//�s���t�F�[�Y
	enum class ActionPhase {
		GooutPlanet,	//�f������o��
		StageSelect,	//�X�e�[�W�I��
		EnterPlanet,	//�f���ɓ���
	};

	//�f���ɓ���s���t�F�[�Y
	enum class EnterPlanetActionPhase {
		Rotate,	//��]
		Stay,	//������Ƒҋ@
		Boost,	//�u�[�X�g
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�o���V�[���p���@</returns>
	static StageSelectPlayer* Create(ObjModel* model, const Vector3& startPosition);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �f������o�Ă���s�����J�n
	/// </summary>
	/// <param name="targetPos">�ڕW���W</param>
	void GooutPlanetStart(const Vector3& targettPos);

	/// <summary>
	/// �X�e�[�W�I���ړ����J�n
	/// </summary>
	/// <param name="targetPos">�ڕW���W</param>
	void StageSelectMoveStart(const Vector3& targetPos, bool isStageSelectMoveRight);

	/// <summary>
	/// �f���ɓ���s�����J�n
	/// </summary>
	/// <param name="selectPlanetPos">�I�������f���̍��W</param>
	void EnterPlanetStart(const Vector3& selectPlanetPos);

	//getter
	ActionPhase GetActionPhase() { return actionPhase; }
	bool GetIsGooutPlanetEnd() { return isGooutPlanetEnd; };
	bool GetIsStageSelectMove() { return isStageSelectMove; }
	bool GetIsStageSelectModeEnd() { return isStageSelectModeEnd; }

private: //�����o�֐�
	/// <summary>
	/// �f������o��s��
	/// </summary>
	void GooutPlanet();

	/// <summary>
	/// �X�e�[�W�I���ړ�
	/// </summary>
	void StageSelectMove();

	/// <summary>
	/// �f���ɓ���s��
	/// </summary>
	void EnterPlanet();

	/// <summary>
	/// �f���ɓ���s��(��])
	/// </summary>
	void EnterPlanetRotate();

	/// <summary>
	/// �f���ɓ���s��(������Ƒҋ@)
	/// </summary>
	void EnterPlanetStay();

	/// <summary>
	/// �f���ɓ���s��(�u�[�X�g)
	/// </summary>
	void EnterPlanetBoost();

private: //�ÓI�����o�ϐ�
	//�s���֐�
	static void (StageSelectPlayer::* actionFuncTable[])();
	//�s���֐�
	static void (StageSelectPlayer::* enterPlanetActionFuncTable[])();

private: //�����o�ϐ�
	//�s��
	ActionPhase actionPhase = ActionPhase::GooutPlanet;
	//�f���ɓ���s��
	EnterPlanetActionPhase enterPlanetActionPhase = EnterPlanetActionPhase::Rotate;
	//�f������o�Ă���s�����I������
	bool isGooutPlanetEnd = false;
	//�X�e�[�W�I���ňړ�����
	bool isStageSelectMove = false;
	//�X�e�[�W�I���ړ����E��
	bool isStageSelectMoveRight = true;
	//�X�e�[�W�I���ōs���S�Ă̍s�����I������
	bool isStageSelectModeEnd = false;
	//�ړ��O���W
	Vector3 moveBeforePos;
	//�ړ�����W
	Vector3 moveAfterPos;
	//�ύX�O�p�x
	Vector3 changeBeforeRota;
	//�ύX��p�x
	Vector3 changeAfterRota;
	//�s���^�C�}�[
	int32_t actionTimer = 0;
	//�p�[�e�B�N���W�F�b�g�̑傫���ύX�p
	int32_t particleJetSizePhaseNum = 0;
};
