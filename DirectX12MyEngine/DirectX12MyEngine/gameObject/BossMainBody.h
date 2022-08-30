#pragma once
#include "ObjObject3d.h"

//GameScene�̑O���錾
class GameScene;

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
	/// <param name="model">���f��</param>
	/// <returns>�{�X(�{��)</returns>
	static BossMainBody* Create(ObjModel* model, const Vector3& basePos);

	//getter
	static const int GetMaxHP() { return BossMainBody::maxHP; }

	//setter
	static void SetGameScene(GameScene* gameScene) { BossMainBody::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { BossMainBody::bulletModel = model; }

public: //�����o�֐�
	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	/// <param name="damageNum">�_���[�W��</param>
	void Damage(int damageNum);

	/// <summary>
	/// �~��
	/// </summary>
	/// <param name="time">�C�[�W���O�p(0�`1)�̐��l</param>
	void Fall(const float time);

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
	/// ��ʒu�ɖ߂�
	/// </summary>
	/// <param name="time">�C�[�W���O�p(0�`1)�̐��l</param>
	void ReturnBasePosition(const float time);

	/// <summary>
	/// �U����Ԃ��I������̂ŕK�v�ȏ����Z�b�g
	/// ��ʒu�ɖ߂�Ƃ��̏o�����W���L�^����A�e���ˏ�ԉ���
	/// </summary>
	void AttackEnd();

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }

private: //�����o�֐�
	/// <summary>
	/// �e����
	/// </summary>
	void Fire(const float scale, const float bulletSpeed);

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
	/// �ҋ@����
	/// </summary>
	void Stay();

private: //�ÓI�����o�ϐ�
	//�Q�[���V�[��
	static GameScene* gameScene;
	//�G�e�̃��f��
	static ObjModel* bulletModel;
	//�̗�
	static const int maxHP = 60;
	//�U����Ԃ�Y����]
	static const float attackModeRotY;
	//�ҋ@��Ԃ�Y����]
	static const float waitModeRotY;
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
	//�e���˃^�C�}�[
	int32_t fireTimer = 0;
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
};
