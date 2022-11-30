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
	enum class AttackTypeAvatarBodyBlowPhase {
		Lockon,
		Shot,
		Back,
		Stay,
	};

	enum class AttackTypeAvatarGatlingPhase {
		MoveCenter,
		RotStart,
		Shot,
		RotEnd,
		Stay,
	};

	enum class AttackTypeAvatarGiantBulletPhase {
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
	static void SetAvatarModel(ObjModel* model) { BossAvatar::avatarModel = model; }
	static void SetAvatarDamageModel(ObjModel* model) { BossAvatar::avatarDamageModel = model; }
	static void SetAvatarSleepModel(ObjModel* model) { BossAvatar::avatarSleepModel = model; }
	static void SetBulletModel(ObjModel* model) { BossAvatar::bulletModel = model; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// ������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	/// <param name="attackPower">�U����</param>
	void Damage(int attackPower, const Vector3& collisionPos);

	/// <summary>
	/// HP�����Ȃ���Ԃ̃��f���ɕύX����
	/// </summary>
	void DamageModelChange();

	/// <summary>
	/// �e���ˍ��W���X�V
	/// </summary>
	void UpdateBulletShotPos();

	/// <summary>
	/// �U�����e:���g�̓�����
	/// </summary>
	void AttackTypeAvatarBodyBlow(const Vector3& playerPosition);

	/// <summary>
	/// �U�����e:���g�K�g�����O�C
	/// </summary>
	void AttackTypeAvatarGatling();

	/// <summary>
	/// �U�����e:���g����e
	/// </summary>
	void AttackTypeAvatarGiantBullet();

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
	/// ��ʒu�ɖ߂�
	/// </summary>
	/// <param name="time">�C�[�W���O�p(0�`1)�̐��l</param>
	void ReturnBasePosition(const float time);

	/// <summary>
	/// �U����Ԃ��I������̂ŕK�v�ȏ����Z�b�g
	/// ��ʒu�ɖ߂�Ƃ��̏o�����W���L�^����A�e���ˏ�ԉ���
	/// </summary>
	void AttackEnd();

	/// <summary>
	/// �Q�Ă����Ԃ̃��f������N���Ă����Ԃ̃��f���ɕύX
	/// </summary>
	void ChangeModel();

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }
	const bool GetIsDelete() { return isDelete; }
	const int GetDamageNum() { return damageNum; }

protected: //�����o�֐�
	/// <summary>
	/// �ʏ�e����
	/// </summary>
	void Fire(const float scale, const float bulletSpeed);

	/// <summary>
	/// �`���[�W�e����
	/// </summary>
	void ChargeBulletFire(const float scale, const float bulletSpeed);

	/// <summary>
	/// �_���[�W��Ԃ̏���
	/// </summary>
	void DamageMode();

	/// <summary>
	/// �_���[�W����
	/// </summary>
	void DamageExplosion(const Vector3& collisionPos);

	/// <summary>
	/// �_���[�W����������Ԃ̐F�ɂ���
	/// </summary>
	void DamageColorMode();

	/// <summary>
	/// �U�����e:���g�̓�����̃��b�N�I������
	/// </summary>
	virtual void AttackTypeAvatarBodyBlowLockon() = 0;

	/// <summary>
	/// �U�����e:���g�̓�����̔�΂�����
	/// </summary>
	void AttackTypeAvatarBodyBlowShot();

	/// <summary>
	/// �U�����e:���g�̓�����̌��̈ʒu�ɖ߂鏈��
	/// </summary>
	void AttackTypeAvatarBodyBlowBack();

	/// <summary>
	/// �U�����e:���g�K�g�����O�C�̒��S�Ɉړ������鏈��
	/// </summary>
	virtual void AttackTypeAvatarGatlingMoveCenter() = 0;

	/// <summary>
	/// �U�����e:���g�K�g�����O�C�̉�]�J�n����
	/// </summary>
	void AttackTypeAvatarGatlingRotStart();

	/// <summary>
	/// �U�����e:���g�K�g�����O�C�̔�΂�����
	/// </summary>
	void AttackTypeAvatarGatlingShot();

	/// <summary>
	/// �U�����e:���g�K�g�����O�C�̉�]�I������
	/// </summary>
	void AttackTypeAvatarGatlingRotEnd();

	/// <summary>
	/// �U�����e:���g����e�̑ҋ@����
	/// </summary>
	virtual void AttackTypeAvatarGiantBulletWait() = 0;

	/// <summary>
	/// �U�����e:���g����e�̈ړ������鏈��
	/// </summary>
	virtual void AttackTypeAvatarGiantBulletMove() = 0;

	/// <summary>
	/// �U�����e:���g����e�̃`���[�W�V���b�g����
	/// </summary>
	void AttackTypeAvatarGiantBulletChargeShot();

	/// <summary>
	/// �U�����e:���g����e�̔����ňړ����鏈��
	/// </summary>
	void AttackTypeAvatarGiantBulletRecoil();

	/// <summary>
	/// �ҋ@����
	/// </summary>
	void Stay();

	/// <summary>
	/// ���S���̏���
	/// </summary>
	void Dead();

	/// <summary>
	/// ���S�����Ƃ��̓���
	/// </summary>
	virtual void DeadAction();

protected: //�ÓI�����o�ϐ�
	//�Q�[���V�[��
	static GameScene* gameScene;
	//���g�̃��f��
	static ObjModel* avatarModel;
	//HP�����Ȃ���Ԃ̃��f��
	static ObjModel* avatarDamageModel;
	//���g�̐Q�Ă����Ԃ̃��f��
	static ObjModel* avatarSleepModel;
	//�G�e�̃��f��
	static ObjModel* bulletModel;
	//�̗�
	static const int maxHP = 15;
	//�U����Ԃ�Y����]
	static const float attackModeRotY;
	//�ҋ@��Ԃ�Y����]
	static const float waitModeRotY;
	//�_���[�W��Ԃ̐F
	static const XMFLOAT4 damageColor;
	//�U�����e:���g�̓�����̍s���J��
	static void (BossAvatar::* attackTypeAvatarBodyBlowPhaseFuncTable[])();
	//�U�����e:���g�K�g�����O�C�̍s���J��
	static void (BossAvatar::* attackTypeAvatarGatlingPhaseFuncTable[])();
	//�U�����e:���g�K�g�����O�C�Ŏg���~�̔��a�̒���
	static const float attackAvatarGatlingLength;
	//�U�����e:���g����e�̍s���J��
	static void (BossAvatar::* attackTypeAvatarGiantBulletPhaseFuncTable[])();

protected: //�����o�ϐ�
	//�̗�
	int HP = maxHP;
	//���S�t���O
	bool isDead = false;
	//�폜�t���O
	bool isDelete = false;
	//��炤�_���[�W��
	int damageNum;
	//�_���[�W�t���O
	bool isDamage = false;
	//�_���[�W��ԃ^�C�}�[
	int32_t damageTimer = 0;
	//�_���[�W�F��
	bool isDamageColor = false;
	//HP�����Ȃ���Ԃ̃��f����
	bool isDamageModel = false;
	//�e���˃^�C�}�[
	int32_t fireTimer = 0;
	//�e���ˍ��W
	Vector3 bulletShotPos;
	//�{�X�{�̂Ƃ̐e�q�֌W��̊���W
	Vector3 basePos;
	//��ʒu�ɖ߂�Ƃ��̏o�����W
	Vector3 returnStartPos;
	//�U���Ŏg���^�C�}�[
	int32_t attackTimer = 0;
	//�U�����e:���g�̓�����̍s��
	AttackTypeAvatarBodyBlowPhase attackAvatarBodyBlowPhase = AttackTypeAvatarBodyBlowPhase::Lockon;
	//�U�����e:���g�̓�����Ŕ�΂��p�x
	Vector3 attackAvatarBodyBlowVelocity;
	//�U�����e:���g�̓�����Ń��b�N�I���ΏۂɂȂ���W
	Vector3 attackAvatarBodyBlowLockonPos;
	//�U�����e:���g�K�g�����O�C�̍s��
	AttackTypeAvatarGatlingPhase attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::MoveCenter;
	//�U�����e:���g�K�g�����O�C�Ŏg���~�^���p�p�x
	float attackAvatarGatlingAngle;
	//�U�����e:���g�K�g�����O�C�Ŏg����]���x
	float attackAvatarGatlingRotSpeed = 0;
	//�U�����e:���g����e�̍s��
	AttackTypeAvatarGiantBulletPhase attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Wait;
	//�U�����e:���g����e�Ŏg���������x
	Vector3 attackAvatarGiantBulletRecoilVelocity = { 0, 0, 2 };
	//�U�����e:���g����e�Ŏg�����������x
	Vector3 attackAvatarGiantBulletRecoilAccel = { 0, 0, -0.05f };
	//���S���ė����x
	Vector3 crashVel = { 0, 0, 0 };
};
