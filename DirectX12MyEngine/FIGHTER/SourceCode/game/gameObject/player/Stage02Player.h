#pragma once
#include "Player.h"

/// <summary>
/// �X�e�[�W02���@
/// </summary>
class Stage02Player : public Player
{
public:
	//�X�e�[�W�N���A��s���t�F�[�Y
	enum class StageClearModePhase {
		SideMove,	//������ړ�
		Return,		//����A��
		Up,			//�㏸
		Stay,		//��~
		Boost,		//�u�[�X�g
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="startHP">�J�n��HP</param>
	/// <param name="maxHP">�ő�HP</param>
	/// <returns>�X�e�[�W02���@</returns>
	static Stage02Player* Create(ObjModel* model, const int startHP, const int maxHP);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(ObjModel* model, const int startHP, const int maxHP) override;

	/// <summary>
	/// �X�e�[�W�N���A��̓������J�n����
	/// </summary>
	void StageClearModeStart();

	/// <summary>
	/// �X�e�[�W�N���A��̋A�҂��J�n����
	/// </summary>
	/// <param name="cameraPos">�J�������W</param>
	void StageClearReturnStart(const Vector3& cameraPos);

	/// <summary>
	/// �X�e�[�W�N���A��̃u�[�X�g���J�n����
	/// </summary>
	void StageClearBoostStart();

	//getter
	const int GetCrashBoundCount() { return crashBoundCount; }
	StageClearModePhase GetStageClearModePhase() { return stageClearModePhase; }

private: //�����o�֐�
	/// <summary>
	/// �ė�
	/// </summary>
	void Crash() override;

	/// <summary>
	/// �X�e�[�W�N���A�s��
	/// </summary>
	void StageClear() override;

	/// <summary>
	/// �X�e�[�W�N���A��̉��ړ�
	/// </summary>
	void StageClearSideMove();

	/// <summary>
	/// �X�e�[�W�N���A��̋A��
	/// </summary>
	void StageClearReturn();

	/// <summary>
	/// �X�e�[�W�N���A��̏㏸
	/// </summary>
	void StageClearUp();

	/// <summary>
	/// �X�e�[�W�N���A��̒�~
	/// </summary>
	void StageClearStay();

	/// <summary>
	/// �X�e�[�W�N���A��̃u�[�X�g
	/// </summary>
	void StageClearBoost();

private: //�ÓI�����o�ϐ�
	//���@�̊���W
	static const Vector3 basePos;
	//�X�e�[�W�N���A��s���J��
	static void (Stage02Player::* stageClearActionFuncTable[])();

private: //�����o�ϐ�
	//�X�e�[�W�N���A��s��
	StageClearModePhase stageClearModePhase = StageClearModePhase::SideMove;
	//�X�e�[�W�N���A��Ɏg�p����^�C�}�[
	int32_t stageClearModeTimer = 0;
	//�X�e�[�W�N���A�ړ��������E��
	bool isStageClearMoveRight = true;
	//�X�e�[�W�N���A�ړ����x
	Vector3 stageClearMoveVelocity;
	//�X�e�[�W�N���A���p�x
	Vector3 stageClearRota;
	//�X�e�[�W�N���A��Ɏg�p����J�����z�[�~���O�p���W
	Vector3 stageClearCameraPos;
	//�J�����z�[�~���O�p���W
	Vector3 cameraHomingPos;
};