#pragma once
#include "ObjObject3d.h"

//�X�e�[�W�V�[���̑O���錾
class BaseStageScene;

/// <summary>
/// �{�X(�{��)
/// </summary>
class BossMainBody : public ObjObject3d
{
private: //�U�����e���Ƃ̃t�F�[�Y
	enum class AttackTypeTrackingPhase {
		Lockon,
		Shot,
		Stay,
	};

	enum class AttackTypeSuperGiantBulletPhase {
		Move,
		ChargeShot,
		Stay,
	};

	enum class AttackTypeRotatePhase {
		Wait,
		Shot,
		Move,
		Stay,
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�{�X(�{��)</returns>
	static BossMainBody* Create(const Vector3& basePos);

	//getter
	static const int GetMaxHP() { return BossMainBody::maxHP; }
	static const Vector3& GetNormalSize() { return BossMainBody::normalSize; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { BossMainBody::stageScene = stageScene; }
	static void SetBossMainBodyModel(ObjModel* model) { BossMainBody::mainBodyModel = model; }
	static void SetBossMainBodyDamageModel(ObjModel* model) { BossMainBody::mainBodyDamageModel = model; }
	static void SetBossMainBodySleepModel(ObjModel* model) { BossMainBody::mainBodySleepModel = model; }
	static void SetBossMainBodyDeadModel(ObjModel* model) { BossMainBody::mainBodyDeadModel = model; }
	static void SetBulletModel(ObjModel* model) { BossMainBody::bulletModel = model; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	/// <param name="attackPower">�U����</param>
	void Damage(int attackPower, const Vector3& collisionPos, const Vector3& subjectVel);

	/// <summary>
	/// HP�����Ȃ���Ԃ̃��f���ɕύX����
	/// </summary>
	void DamageModelChange();

	/// <summary>
	/// �~��
	/// </summary>
	/// <param name="time">�C�[�W���O�p(0�`1)�̐��l</param>
	void Fall(const float time);

	/// <summary>
	/// �e���ˍ��W���X�V
	/// </summary>
	void UpdateBulletShotPos();

	/// <summary>
	/// �U�����e:�Ǐ]
	/// </summary>
	void AttackTypeTracking(const Vector3& playerPosition);

	/// <summary>
	/// �U�����e:������e
	/// </summary>
	void AttackTypeSuperGiantBullet();

	/// <summary>
	/// �U�����e:��]
	/// </summary>
	void AttackTypeRotate();

	/// <summary>
	/// �U�����e:���g�̓�����
	/// </summary>
	void AttackTypeAvatarBodyBlow();

	/// <summary>
	/// �U�����e:���g�K�g�����O�C
	/// </summary>
	void AttackTypeAvatarGatling(const Vector3& playerPosition);

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
	/// ���S���ʒu�ɖ߂�
	/// </summary>
	void DeadBasePosBack(const float time);

	/// <summary>
	/// ���S����
	/// </summary>
	void DeadFall();

	/// <summary>
	/// �Q�Ă����Ԃ̃��f������N���Ă����Ԃ̃��f���ɕύX
	/// </summary>
	void ChangeModel();

	/// <summary>
	/// ���S��Ԃ̃��f���ɕύX
	/// </summary>
	void ChangeDeadModel();

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }
	const bool GetIsDelete() { return isDelete; }
	const int GetDamageNum() { return damageNum; }
	const bool GetIsDamageTrigger() { return isDamageTrigger; }

private: //�����o�֐�
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
	void DamageExplosion(const Vector3& position);

	/// <summary>
	/// �_���[�W����������Ԃ̐F�ɂ���
	/// </summary>
	void DamageColorMode();

	/// <summary>
	/// �U�����e:�Ǐ]�̃��b�N�I������
	/// </summary>
	void AttackTypeTrackingLockon();

	/// <summary>
	/// �U�����e:�Ǐ]�̒e���ˏ���
	/// </summary>
	void AttackTypeTrackingShot();

	/// <summary>
	/// �U�����e:������e�̈ړ�����
	/// </summary>
	void AttackTypeSuperGiantBulletMove();

	/// <summary>
	/// �U�����e:������e�̃`���[�W�V���b�g����
	/// </summary>
	void AttackTypeSuperGiantBulletChargeShot();

	/// <summary>
	/// �U�����e:��]�̑ҋ@����
	/// </summary>
	void AttackTypeRotateWait();

	/// <summary>
	/// �U�����e:��]�̒e���ˏ���
	/// </summary>
	void AttackTypeRotateShot();

	/// <summary>
	/// �U�����e:��]�̈ړ�����
	/// </summary>
	void AttackTypeRotateMove();

	/// <summary>
	/// ���S�������̔���
	/// </summary>
	void DeadFallExplosion();

	/// <summary>
	/// �ҋ@����
	/// </summary>
	void Stay();

private: //�ÓI�����o�ϐ�
	//�X�e�[�W�V�[��
	static BaseStageScene* stageScene;
	//�{�̂̃��f��
	static ObjModel* mainBodyModel;
	//HP�����Ȃ���Ԃ̃��f��
	static ObjModel* mainBodyDamageModel;
	//�{�̂̐Q�Ă����Ԃ̃��f��
	static ObjModel* mainBodySleepModel;
	//�{�̂̎��S��Ԃ̃��f��
	static ObjModel* mainBodyDeadModel;
	//�G�e�̃��f��
	static ObjModel* bulletModel;
	//�ʏ�T�C�Y
	static const Vector3 normalSize;
	//�_���[�W��Ԃ̃T�C�Y
	static const Vector3 damageSize;
	//�̗�
	static const int maxHP = 30;
	//�U����Ԃ�Y����]
	static const float attackModeRotY;
	//�ҋ@��Ԃ�Y����]
	static const float waitModeRotY;
	//�_���[�W��Ԃ̐F
	static const XMFLOAT4 damageColor;
	//�U�����e:�Ǐ]�̍s���J��
	static void (BossMainBody::* attackTypeTrackingPhaseFuncTable[])();
	//�U�����e:������e�̍s���J��
	static void (BossMainBody::* attackTypeSuperGiantBulletPhaseFuncTable[])();
	//�U�����e:��]�̍s���J��
	static void (BossMainBody::* attackTypeRotatePhaseFuncTable[])();

private: //�����o�ϐ�
	//��~�����̍��W
	Vector3 basePos;
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
	//��ʒu�ɖ߂�Ƃ��̏o�����W
	Vector3 returnStartPos;
	//�U���Ŏg���^�C�}�[
	int32_t attackTimer = 0;
	//�U�����e:�Ǐ]�̍s��
	AttackTypeTrackingPhase attackTrackingPhase = AttackTypeTrackingPhase::Lockon;
	//�U�����e:������e�̍s��
	AttackTypeSuperGiantBulletPhase attackSuperGiantBulletPhase = AttackTypeSuperGiantBulletPhase::Move;
	//�U�����e:��]�̍s��
	AttackTypeRotatePhase attackRotatePhase = AttackTypeRotatePhase::Wait;
	//�U�����e:��]�Ŏg���U����
	int attackRotateShotCount = 0;
	//�U�����e:��]�ňړ��O���W
	Vector3 attackRotateMoveBeforePos;
	//�U�����e:��]�ňړ�����W
	Vector3 attackRotateMpveAfterPos;
	//���S���̍��W
	Vector3 deadPos;
	//���S���̊p�x
	Vector3 deadRota;
	//���S���������x
	Vector3 deadFallVel;
	//���S�����������^�C�}�[
	int32_t deadFallExplosionTimer = 0;
};
