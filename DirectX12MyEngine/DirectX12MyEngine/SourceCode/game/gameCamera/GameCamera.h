#pragma once
#include <DirectXMath.h>
#include "Camera.h"

//���@�N���X��O���錾
class Player;
//�{�X�{�̂�O���錾
class BossMainBody;

/// <summary>
/// �Q�[���J����
/// </summary>
class GameCamera : public Camera
{
public:
	//�X�e�[�W�N���A��s���t�F�[�Y
	enum class StageClearModePhase {
		BossLook,		//�{�X�̕���������
		FrontLock,		//���ʂ̕���������
		PlayerLock,		//���@�̕���������
		PlayerZoom,		//���@���Y�[��
		PlayerFollow,	//���@��Ǐ](���ۂ͂Ȃɂ����Ȃ�)
		PlayerSideMove,	//���@���Ɉړ�
		PlayerKeepLock,	//���@�̕����������ƌ���
	};

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �ė��J�n
	/// </summary>
	void CrashStart();

	/// <summary>
	/// �V�F�C�N�J�n
	/// </summary>
	void ShakeStart();

	/// <summary>
	/// �X�e�[�W�N���A��̓������J�n����
	/// </summary>
	/// <param name="bossMainBody">�{�X�{��</param>
	void StageClearModeStart(BossMainBody* bossMainBody);

	/// <summary>
	/// �{�X���폜���ꂽ��
	/// </summary>
	void BossDelete();

	//getter
	const Vector3& GetPosition() { return position; }

	//setter
	void SetPlayer(Player* player) { this->player = player; }
	void SetIsAdvance(const bool isAdvance) { this->isAdvance = isAdvance; }
	StageClearModePhase GetStageClearModePhase() { return stageClearModePhase; }

private: //�����o�֐�
	/// <summary>
	/// �J��������
	/// </summary>
	void CameraAction();

	/// <summary>
	/// �ė�
	/// </summary>
	void Crash();

	/// <summary>
	/// ��]
	/// </summary>
	void Rotate();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �v���C���[�ɍ��킹�ăm�b�N�o�b�N����
	/// </summary>
	void Knockback();

	/// <summary>
	/// �V�F�C�N
	/// </summary>
	void Shake();

	/// <summary>
	/// �X�e�[�W�N���A��̃{�X�̕����������s��
	/// </summary>
	void StageClearBossLock();

	/// <summary>
	/// �X�e�[�W�N���A��̐��ʂ̕����������s��
	/// </summary>
	void StageClearFrontLock();

	/// <summary>
	/// �X�e�[�W�N���A��̎��@�̕����������s��
	/// </summary>
	void StageClearPlayerLock();

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
	void StageClearPlayerKeepLock();


private: //�ÓI�����o�ϐ�
	//�X�e�[�W�N���A��s���J��
	static void (GameCamera::* stageClearCameraActionFuncTable[])();

private: //�����o�ϐ�
	//�v���C���[���@
	Player* player = nullptr;
	//�{�X�{��
	BossMainBody* bossMainBody = nullptr;
	//��]
	Vector3 rotation = { 0, 0, 0 };
	//���W
	Vector3 position = { 0, 0, 0 };
	//�O�i���邩
	bool isAdvance = true;
	//�ė���Ԃ�
	bool isCrash = false;
	//�ė���Ԃ̃J�����ʒu�Ɉړ����邩
	bool isMoveCrashPos = false;
	//�ė���Ԃ̃J�����ʒu�Ɉړ��^�C�}�[
	int32_t moveCrashPosTimer = 0;
	//�ė���Ԃ̃J�����ʒu�Ɉړ��O�̍��W
	Vector3 moveCrashBeforePos;
	//�V�F�C�N���邩
	bool isShake = false;
	//�J�����V�F�C�N�p�^�C�}�[
	int32_t shakeTimer = 0;
	//�X�e�[�W�N���A��̓��������邩
	bool isStageClearMode = false;
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