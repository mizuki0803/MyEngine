#pragma once
#include "ObjObject3d.h"

//�X�e�[�W�V�[���̑O���錾
class BaseStageScene;

/// <summary>
/// ���_�}�[��(���g)
/// </summary>
class MedamanAvatar : public ObjObject3d
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
	static const int GetMaxHP() { return MedamanAvatar::maxHP; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { MedamanAvatar::stageScene = stageScene; }
	static void SetAvatarModel(ObjModel* model) { MedamanAvatar::avatarModel = model; }
	static void SetAvatarDamageModel(ObjModel* model) { MedamanAvatar::avatarDamageModel = model; }
	static void SetAvatarSleepModel(ObjModel* model) { MedamanAvatar::avatarSleepModel = model; }
	static void SetAvatarDeadModel(ObjModel* model) { MedamanAvatar::avatarDeadModel = model; }
	static void SetBulletModel(ObjModel* model) { MedamanAvatar::bulletModel = model; }

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
	/// <param name="collisionPos">�Փˍ��W</param>
	/// <param name="subjectVel">�Փ˂�������Ƃ̃x�N�g��</param>
	void Damage(int attackPower, const Vector3& collisionPos, const Vector3& subjectVel);

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
	/// <param name="playerPosition">���@���W</param>
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
	/// ��ʒu�ɖ߂�Ƃ��̏o�����W���L�^����
	/// </summary>
	void SetReturnBasePosition();

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
	const bool GetIsDead() { return isDead; }
	const bool GetIsDelete() { return isDelete; }
	const int GetDamageNum() { return damageNum; }
	const bool GetIsDamageTrigger() { return isDamageTrigger; }

protected: //�����o�֐�
	/// <summary>
	/// �ʏ�e����
	/// </summary>
	/// <param name="scale">�e�̑傫��</param>
	/// <param name="bulletSpeed">�e�̑��x</param>
	void Fire(const float scale, const float bulletSpeed);

	/// <summary>
	/// �`���[�W�e����
	/// </summary>
	/// <param name="scale">�e�̑傫��</param>
	/// <param name="bulletSpeed">�e�̑��x</param>
	void ChargeBulletFire(const float scale, const float bulletSpeed);

	/// <summary>
	/// �_���[�W��Ԃ̏���
	/// </summary>
	void DamageMode();

	/// <summary>
	/// �_���[�W��Ԃ̃m�b�N�o�b�N�����Z�b�g
	/// </summary>
	/// <param name="subjectVel">�Ώۂ̑��x</param>
	void SetDamageKnockback(const Vector3& subjectVel);

	/// <summary>
	/// �_���[�W��Ԃ̃m�b�N�o�b�N
	/// </summary>
	void DamageKnockback();

	/// <summary>
	/// �_���[�W��Ԃő傫�������T�C�Y��߂��Ă�������
	/// </summary>
	void DamageSizeReturn();

	/// <summary>
	/// �_���[�W����
	/// </summary>
	/// <param name="collisionPos">�Փˍ��W</param>
	void DamageExplosion(const Vector3& collisionPos);

	/// <summary>
	/// �_���[�W����������Ԃ̐F���`�J�`�J�����鏈��
	/// </summary>
	void DamageColorChange();

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

	/// <summary>
	/// ���S��̍����G�t�F�N�g
	/// </summary>
	void DeadBlackSmoke();

protected: //�ÓI�����o�ϐ�
	//�X�e�[�W�V�[��
	static BaseStageScene* stageScene;
	//���g�̃��f��
	static ObjModel* avatarModel;
	//HP�����Ȃ���Ԃ̃��f��
	static ObjModel* avatarDamageModel;
	//���g�̐Q�Ă����Ԃ̃��f��
	static ObjModel* avatarSleepModel;
	//���S��Ԃ̃��f��
	static ObjModel* avatarDeadModel;
	//�G�e�̃��f��
	static ObjModel* bulletModel;
	//�ʏ�T�C�Y
	static const Vector3 normalSize;
	//�_���[�W��Ԃ̃T�C�Y
	static const Vector3 damageSize;
	//�̗�
	static const int maxHP = 8;
	//�U����Ԃ�Y����]
	static const float attackModeRotY;
	//�ҋ@��Ԃ�Y����]
	static const float waitModeRotY;
	//�_���[�W��Ԃ̐F
	static const XMFLOAT4 damageColor;
	//�U�����e:���g�̓�����̍s���J��
	static void (MedamanAvatar::* attackTypeAvatarBodyBlowPhaseFuncTable[])();
	//�U�����e:���g�K�g�����O�C�̍s���J��
	static void (MedamanAvatar::* attackTypeAvatarGatlingPhaseFuncTable[])();
	//�U�����e:���g�K�g�����O�C�Ŏg���~�̔��a�̒���
	static const float attackAvatarGatlingLength;
	//�U�����e:���g����e�̍s���J��
	static void (MedamanAvatar::* attackTypeAvatarGiantBulletPhaseFuncTable[])();

protected: //�����o�ϐ�
	//�̗�
	int HP = maxHP;
	//���S�t���O
	bool isDead = false;
	//���S���Ă���̎��ԃ^�C�}�[
	int32_t deadTimer = 0;
	//�폜�t���O
	bool isDelete = false;
	//��炤�_���[�W��
	int damageNum;
	//�_���[�W�t���O
	bool isDamage = false;
	//�_���[�W���������u�Ԃ�
	bool isDamageTrigger = false;
	//�_���[�W��ԃ^�C�}�[
	int32_t damageTimer = 0;
	//�_���[�W�F��
	bool isDamageColor = false;
	//HP�����Ȃ���Ԃ̃��f����
	bool isDamageModel = false;
	//�m�b�N�o�b�N����
	Vector3 knockbackVec;
	//�m�b�N�o�b�N���x
	Vector3 knockbackVel;
	//�e���˃^�C�}�[
	int32_t fireTimer = 0;
	//�e���ˍ��W
	Vector3 bulletShotPos;
	//���_�}�[���{�̂Ƃ̐e�q�֌W��̊���W
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
