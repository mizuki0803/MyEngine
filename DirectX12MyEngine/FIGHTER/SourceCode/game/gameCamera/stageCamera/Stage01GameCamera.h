#pragma once
#include "BaseGameCamera.h"

//�X�e�[�W01���@�N���X��O���錾
class Stage01Player;
//�{�X�{�̂�O���錾
class BossMainBody;

/// <summary>
/// �X�e�[�W01�Q�[���J����
/// </summary>
class Stage01GameCamera : public BaseGameCamera
{
public:
	//�X�e�[�W�N���A��s���t�F�[�Y
	enum class StageClearModePhase {
		BossLook,		//�{�X�̕���������
		FrontLook,		//���ʂ̕���������
		PlayerLook,		//���@�̕���������
		PlayerZoom,		//���@���Y�[��
		PlayerFollow,	//���@��Ǐ](���ۂ͂Ȃɂ����Ȃ�)
		PlayerSideMove,	//���@���Ɉړ�
		PlayerKeepLook,	//���@�̕����������ƌ���
	};

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="player">���@</param>
	void Initialize(Stage01Player* player);

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
	/// <param name="bossMainBody">�{�X�{��</param>
	void StageClearModeStart(BossMainBody* bossMainBody);

	/// <summary>
	/// �{�X���폜���ꂽ��
	/// </summary>
	void BossDelete();

	//setter
	StageClearModePhase GetStageClearModePhase() { return stageClearModePhase; }

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
	/// �X�e�[�W�N���A��̃{�X�̕����������s��
	/// </summary>
	void StageClearBossLook();

	/// <summary>
	/// �X�e�[�W�N���A��̐��ʂ̕����������s��
	/// </summary>
	void StageClearFrontLook();

	/// <summary>
	/// �X�e�[�W�N���A��̎��@�̕����������s��
	/// </summary>
	void StageClearPlayerLook();

	/// <summary>
	/// �X�e�[�W�N���A��̎��@���Y�[������s��
	/// </summary>
	void StageClearPlayerZoom();

	/// <summary>
	/// �X�e�[�W�N���A��̎��@�Ǐ]����s��
	/// </summary>
	void StageClearPlayerFollow();

	/// <summary>
	/// �X�e�[�W�N���A��̎��@���Ɉړ�����s��
	/// </summary>
	void StageClearPlayerSideMove();

	/// <summary>
	/// �X�e�[�W�N���A��̎��@�̕����������ƌ����s��
	/// </summary>
	void StageClearPlayerKeepLook();


private: //�ÓI�����o�ϐ�
	//�X�e�[�W�N���A��s���J��
	static void (Stage01GameCamera::* stageClearCameraActionFuncTable[])();

private: //�����o�ϐ�
	//�X�e�[�W01���@
	Stage01Player* player = nullptr;
	//�{�X�{��
	BossMainBody* bossMainBody = nullptr;
	//�ė���Ԃ̃J�����ʒu�Ɉړ����邩
	bool isMoveCrashPos = false;
	//�ė���Ԃ̃J�����ʒu�Ɉړ��^�C�}�[
	int32_t moveCrashPosTimer = 0;
	//�ė���Ԃ̃J�����ʒu�Ɉړ��O�̍��W
	Vector3 moveCrashBeforePos;
	//�X�e�[�W�N���A��J��������
	StageClearModePhase stageClearModePhase = StageClearModePhase::BossLook;
	//�X�e�[�W�N���A��̃J���������Ɏg�p����^�C�}�[
	int32_t stageClearModeTimer = 0;
	//�{�X�̕����������Ƃ���Y���W
	float bossLookPosY = 0;
	//�X�e�[�W�N���A��̈ړ��O���W
	Vector3 stageClearMoveBeforePos;
	//�X�e�[�W�N���A��̈ړ��O�J�����p�x
	Vector3 stageClearMoveBeforeRota;
};