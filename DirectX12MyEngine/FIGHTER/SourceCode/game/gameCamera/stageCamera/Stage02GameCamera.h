#pragma once
#include "BaseGameCamera.h"

//�X�e�[�W02���@�N���X��O���錾
class Stage02Player;
//�M�����N�V�[���̂�O���錾
class GalaxyBody;

/// <summary>
/// �X�e�[�W02�Q�[���J����
/// </summary>
class Stage02GameCamera : public BaseGameCamera
{
public:
	//�X�e�[�W�N���A��s���t�F�[�Y
	enum class StageClearModePhase {
		BossSide,		//�{�X�̉��Ɉړ�
		BossBack,		//�{�X�̌��Ɉړ�
		PlayerFront,	//���@�𐳖ʂɈړ�
		PlayerAround,	//���@�̎�������
		PlayerBoostPos,	//���@�̃u�[�X�g������ʒu�Ɉړ�
	};

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="player">���@</param>
	void Initialize(Stage02Player* player);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �ė��J�n
	/// </summary>
	void CrashStart();

	/// <summary>
	/// �X�e�[�W�N���A��̓������J�n����
	/// </summary>
	/// <param name="bossMainBody">�{�X����</param>
	void StageClearModeStart(GalaxyBody* galaxyBody);

	/// <summary>
	/// �X�e�[�W�N���A��̎��@�̐��ʂɈړ�����s�����J�n����
	/// </summary>
	void StageClearPlayerFrontStart();

	/// <summary>
	/// �X�e�[�W�N���A��̎��@�u�[�X�g������ʒu�Ɉړ�����s�����J�n����
	/// </summary>
	void StageClearPlayerBoostLookPosStart();

	/// <summary>
	/// �{�X�̏����폜
	/// </summary>
	void BossDelete();

	//setter
	StageClearModePhase GetStageClearModePhase() { return stageClearModePhase; }
	const float GetPlayerAroundRotAngle() { return playerAroundRotAngle; };
	const bool GetIsStageClearModeCompletion()  { return isStageClearModeCompletion; }

private: //�����o�֐�
	/// <summary>
	/// �ė�
	/// </summary>
	void Crash() override;

	/// <summary>
	/// �X�e�[�W�N���A��s��
	/// </summary>
	void StageClear() override;

	/// <summary>
	/// �X�e�[�W�N���A��̃{�X�̉��Ɉړ�����s��
	/// </summary>
	void StageClearBossSide();

	/// <summary>
	/// �X�e�[�W�N���A��̃{�X�̌��Ɉړ�����s��
	/// </summary>
	void StageClearBossBack();

	/// <summary>
	/// �X�e�[�W�N���A��̎��@�̐��ʂɈړ�����s��
	/// </summary>
	void StageClearPlayerFront();

	/// <summary>
	/// �X�e�[�W�N���A��̎��@�̎�������s��
	/// </summary>
	void StageClearPlayerAround();

	/// <summary>
	/// �X�e�[�W�N���A��̎��@�u�[�X�g������ʒu�Ɉړ�����s��
	/// </summary>
	void StageClearPlayerBoostLookPos();


private: //�ÓI�����o�ϐ�
	//�X�e�[�W�N���A��s���J��
	static void (Stage02GameCamera::* stageClearCameraActionFuncTable[])();

private: //�����o�ϐ�
	//�X�e�[�W01���@
	Stage02Player* player = nullptr;
	//�{�X����
	GalaxyBody* galaxyBody = nullptr;
	//�ė���Ԃ̃J�����ʒu�Ɉړ����邩
	bool isMoveCrashPos = false;
	//�ė��p�̎��@�̒��S���W����̋���
	Vector3 crashPlayerDistance = {};
	//�ė���Ԃ̃J�����ʒu�Ɉړ��^�C�}�[
	int32_t moveCrashPosTimer = 0;
	//�ė���Ԃ̃J�����ʒu�Ɉړ��O�̍��W
	Vector3 moveCrashBeforePos;
	//�ė���Ԃ̃J�����ʒu�Ɉړ��O�̊p�x
	Vector3 moveCrashBeforeRota;
	//�X�e�[�W�N���A��J��������
	StageClearModePhase stageClearModePhase = StageClearModePhase::BossSide;
	//�X�e�[�W�N���A��̃J���������Ɏg�p����^�C�}�[
	int32_t stageClearModeTimer = 0;
	//�X�e�[�W�N���A��̈ړ��O���W
	Vector3 stageClearMoveBeforePos;
	//�X�e�[�W�N���A��̈ړ��O�J�����p�x
	Vector3 stageClearMoveBeforeRota;
	//���@��������Ƃ��̉�]�p�x
	float playerAroundRotAngle = 0;
	//���@��������Ƃ��̎��@�Ƃ̋���
	float playerAroundDistance = 0;
	//�ړ��O�̎��@��������Ƃ��̉�]�p�x
	float beforePlayerAroundRotAngle = 0;
	//�ړ��O�̎��@��������Ƃ��̎��@�Ƃ̋���
	float beforePlayerAroundDistance = 0;
	//�J�����̃X�e�[�W�N���A�オ����������
	bool isStageClearModeCompletion = false;
};