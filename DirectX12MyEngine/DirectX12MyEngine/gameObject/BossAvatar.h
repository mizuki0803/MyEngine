#pragma once
#include "ObjObject3d.h"

//GameScene�̑O���錾
class GameScene;

/// <summary>
/// �{�X(���g)
/// </summary>
class BossAvatar : public ObjObject3d
{
protected: //�U�����e���Ƃ̃t�F�[�Y
	enum class AttackTypeBPhase {
		Lockon,
		Shot,
		Back,
		Stay,
	};

	enum class AttackTypeCPhase {
		MoveCenter,
		RotStart,
		Shot,
		RotEnd,
		Stay,
	};

	enum class AttackTypeDPhase {
		Wait,
		Move,
		ChargeShot,
		Recoil,
		Stay,
	};

public: //�ÓI�����o�֐�
	//getter
	static const int GetMaxHP() { return BossAvatar::maxHP; }

	//setter
	static void SetGameScene(GameScene* gameScene) { BossAvatar::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { BossAvatar::bulletModel = model; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	/// <param name="damageNum">�_���[�W��</param>
	void Damage(int damageNum);

	/// <summary>
	/// �U�����eB
	/// </summary>
	void AttackTypeB(const Vector3& playerPosition);

	/// <summary>
	/// �U�����eC
	/// </summary>
	void AttackTypeC();

	/// <summary>
	/// �U�����eD
	/// </summary>
	void AttackTypeD();

	/// <summary>
	/// �U����ԂɕύX
	/// </summary>
	/// <param name="time">�C�[�W���O�p(0�`1)�̐��l</param>
	void ChangeAttackMode(const float time);

	/// <summary>
	/// �ҋ@��ԂɕύX
	/// </summary>
	/// <param name="time">�C�[�W���O�p(0�`1)�̐��l</param>
	void ChangeWaitMode(const float time);

	/// <summary>
	/// �Œ�ʒu�ɖ߂�
	/// </summary>
	/// <param name="time">�C�[�W���O�p(0�`1)�̐��l</param>
	void ReturnFixedPosition(const float time);

	/// <summary>
	/// �U����Ԃ��I������̂ŕK�v�ȏ����Z�b�g
	/// �Œ�ʒu�ɖ߂�Ƃ��̏o�����W���L�^����A�e���ˏ�ԉ���
	/// </summary>
	void AttackEnd();

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }

protected:
	/// <summary>
	/// �e����
	/// </summary>
	void Fire(const float scale, const float bulletSpeed);

	/// <summary>
	/// �U�����eB�̃��b�N�I������
	/// </summary>
	virtual void AttackTypeBLockon() = 0;

	/// <summary>
	/// �U�����eB�̔�΂�����
	/// </summary>
	void AttackTypeBShot();

	/// <summary>
	/// �U�����eB�̌��̈ʒu�ɖ߂鏈��
	/// </summary>
	void AttackTypeBBack();

	/// <summary>
	/// �U�����eC�̒��S�Ɉړ������鏈��
	/// </summary>
	virtual void AttackTypeCMoveCenter() = 0;

	/// <summary>
	/// �U�����eC�̉�]�J�n����
	/// </summary>
	void AttackTypeCRotStart();

	/// <summary>
	/// �U�����eC�̔�΂�����
	/// </summary>
	void AttackTypeCShot();

	/// <summary>
	/// �U�����eC�̉�]�I������
	/// </summary>
	void AttackTypeCRotEnd();

	/// <summary>
	/// �U�����eD�̑ҋ@����
	/// </summary>
	virtual void AttackTypeDWait() = 0;

	/// <summary>
	/// �U�����eD�̈ړ������鏈��
	/// </summary>
	virtual void AttackTypeDMove() = 0;

	/// <summary>
	/// �U�����eD�̃`���[�W�V���b�g����
	/// </summary>
	void AttackTypeDChargeShot();

	/// <summary>
	/// �U�����eD�̔����ňړ����鏈��
	/// </summary>
	void AttackTypeDRecoil();

	/// <summary>
	/// �ҋ@����
	/// </summary>
	void Stay();

protected: //�ÓI�����o�ϐ�
	//�Q�[���V�[��
	static GameScene* gameScene;
	//�G�e�̃��f��
	static ObjModel* bulletModel;
	//�̗�
	static const int maxHP = 1;
	//�U����Ԃ�Y����]
	static const float attackModeRotY;
	//�ҋ@��Ԃ�Y����]
	static const float waitModeRotY;
	//�U�����eB�̍s���J��
	static void (BossAvatar::* attackTypeBPhaseFuncTable[])();
	//�U�����eC�̍s���J��
	static void (BossAvatar::* attackTypeCPhaseFuncTable[])();
	//�U�����eC�Ŏg���~�̔��a�̒���
	static const float attackCLength;
	//�U�����eD�̍s���J��
	static void (BossAvatar::* attackTypeDPhaseFuncTable[])();

protected: //�����o�ϐ�
	//�̗�
	int HP = maxHP;
	//���S�t���O
	bool isDead = false;
	//�e���˃^�C�}�[
	int32_t fireTimer = 0;
	//�{�X�{�̂Ƃ̐e�q�֌W��̒�ʒu���W
	Vector3 fixedPos;
	//�Œ�ʒu�ɖ߂�Ƃ��̏o�����W
	Vector3 returnStartPos;
	//�U�����eB�̍s��
	AttackTypeBPhase attackBPhase = AttackTypeBPhase::Lockon;
	//�U�����eB�Ŏg���^�C�}�[
	int32_t attackBTimer = 0;
	//�U�����eB�Ŕ�΂��p�x
	Vector3 attackBVelocity;
	//�U�����eB�Ń��b�N�I���ΏۂɂȂ���W
	Vector3 attackBLockonPos;
	//�U�����eC�̍s��
	AttackTypeCPhase attackCPhase = AttackTypeCPhase::MoveCenter;
	//�U�����eC�Ŏg���^�C�}�[
	int32_t attackCTimer = 0;
	//�U�����eC�Ŏg���~�^���p�p�x
	float attackCAngle;
	//�U�����eC�Ŏg����]���x
	float attackCRotSpeed = 0;
	//�U�����eD�̍s��
	AttackTypeDPhase attackDPhase = AttackTypeDPhase::Wait;
	//�U�����eD�Ŏg���^�C�}�[
	int32_t attackDTimer = 0;
	//�U�����eD�Ŏg���������x
	Vector3 attackDRecoilVelocity = { 0, 0, 2 };
	//�U�����eD�Ŏg�����������x
	Vector3 attackDRecoilAccel = { 0, 0, -0.05f };
};
