#pragma once
#include "BasePlayer.h"

/// <summary>
/// �X�e�[�W01���@
/// </summary>
class Stage01Player : public BasePlayer
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
	/// <param name="isVaporCreate">��s�@�_���o�𐶐����邩</param>
	/// <returns>�X�e�[�W01���@</returns>
	static Stage01Player* Create(ObjModel* model, const int startHP, const int maxHP, const bool isVaporCreate);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="startHP">�J�n��HP</param>
	/// <param name="maxHP">�ő�HP</param>
	/// <param name="isVaporCreate">��s�@�_���o�𐶐����邩</param>
	/// <returns>����</returns>
	bool Initialize(ObjModel* model, const int startHP, const int maxHP, const bool isVaporCreate) override;

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
	/// ��]
	/// </summary>
	void Rotate() override;

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
	//�X�e�[�W�N���A��s���J��
	static void (Stage01Player::* stageClearActionFuncTable[])();

private: //�����o�ϐ�
	//�ė��o�E���h��
	int crashBoundCount = 0;
	//�ė����x
	Vector3 crashVel = { 0, 0, 0.25f };
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
