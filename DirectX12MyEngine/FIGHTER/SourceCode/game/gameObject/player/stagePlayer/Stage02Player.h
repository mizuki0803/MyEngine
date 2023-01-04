#pragma once
#include "BasePlayer.h"

/// <summary>
/// �X�e�[�W02���@
/// </summary>
class Stage02Player : public BasePlayer
{
public:
	//�X�e�[�W�N���A��s���t�F�[�Y
	enum class StageClearModePhase {
		Stay,		//��~
		Advance,	//�O�i
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
	/// <returns>�X�e�[�W02���@</returns>
	static Stage02Player* Create(ObjModel* model, const int startHP, const int maxHP, const bool isVaporCreate);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(ObjModel* model, const int startHP, const int maxHP, const bool isVaporCreate) override;

	/// <summary>
	/// �X�e�[�W�N���A��̓������J�n����
	/// </summary>
	void StageClearModeStart();

	//getter
	StageClearModePhase GetStageClearModePhase() { return stageClearModePhase; }

	//setter
	void SetStageClearModePhase(StageClearModePhase stageClearModePhase) { this->stageClearModePhase = stageClearModePhase; }

private: //�����o�֐�
	/// <summary>
	/// �ė��J�n
	/// </summary>
	void CrashStart() override;

	/// <summary>
	/// �ė�
	/// </summary>
	void Crash() override;

	/// <summary>
	/// �X�e�[�W�N���A�s��
	/// </summary>
	void StageClear() override;

	/// <summary>
	/// �X�e�[�W�N���A��̒�~
	/// </summary>
	void StageClearStay();

	/// <summary>
	/// ��]�p��0�ɏC��
	/// </summary>
	void StageClearRotateFix();

	/// <summary>
	/// �X�e�[�W�N���A��̑O�i
	/// </summary>
	void StageClearAdvance();

	/// <summary>
	/// �X�e�[�W�N���A��̃u�[�X�g
	/// </summary>
	void StageClearBoost();

private: //�ÓI�����o�ϐ�
	//�X�e�[�W�N���A��s���J��
	static void (Stage02Player::* stageClearActionFuncTable[])();

private: //�����o�ϐ�
	//�ė����Ă���̎��ԃ^�C�}�[
	int32_t crashTimer = 0;
	//�X�e�[�W�N���A��s��
	StageClearModePhase stageClearModePhase = StageClearModePhase::Stay;
	//�X�e�[�W�N���A��Ɏg�p����^�C�}�[
	int32_t stageClearModeTimer = 0;
	//�X�e�[�W�N���A�ړ����x
	Vector3 stageClearMoveVelocity;
	//�X�e�[�W�N���A���p�x
	Vector3 stageClearRota;
};
