#pragma once
#include "ObjObject3d.h"

//�X�e�[�W�V�[���̑O���錾
class BaseStageScene;

/// <summary>
/// �M�����N�V�[(��C)
/// </summary>
class GalaxyCannon : public ObjObject3d
{
private: //�U�����e�̃t�F�[�Y
	//����
	enum class AttackTypeRapidFirePhase {
		StartWait,//�J�n���̑ҋ@
		Charge,	//����
		Shot,	//����
		Return,	//�߂�
		Wait,	//�ҋ@
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="parent">�e�I�u�W�F�N�g</param> 
	/// <param name="basePos">����W</param>
	/// <returns>�M�����N�V�[(��C)</returns>
	static GalaxyCannon* Create(ObjObject3d* parent, const Vector3& basePos);

	//getter
	static const int GetMaxHP() { return GalaxyCannon::maxHP; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { GalaxyCannon::stageScene = stageScene; }
	static void SetCannonModel(ObjModel* model) { GalaxyCannon::cannonModel = model; }
	static void SetCannonDeadModel(ObjModel* model) { GalaxyCannon::cannonDeadModel = model; }
	static void SetBulletModel(ObjModel* model) { GalaxyCannon::bulletModel = model; }
	static void SetBreakModel(int modelNum, ObjModel* model);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	/// <param name="attackPower">�U����</param>
	/// <param name="collisionPos">�Փˍ��W</param>
	void Damage(int attackPower, const Vector3& collisionPos);

	/// <summary>
	/// �e���ˍ��W���X�V
	/// </summary>
	void UpdateBulletShotPos();

	/// <summary>
	/// �U����ԊJ�n����
	/// </summary>
	void AttackModeStart();

	/// <summary>
	/// �U�����e:����
	/// </summary>
	void AttackTypeRapidFire();

	//getter	
	const bool GetIsDead() { return isDead; }
	const int GetDamageNum() { return damageNum; }
	const bool GetIsDamageTrigger() { return isDamageTrigger; }
	const bool GetIsAttack() { return isAttack; }

	//setter
	void SetRapidFireStartWaitTime(const int startWaitTime) { rapidFireStartWaitTime = startWaitTime; }

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
	/// �_���[�W��Ԃő傫�������T�C�Y��߂��Ă�������
	/// </summary>
	void DamageSizeReturn();

	/// <summary>
	/// �_���[�W����
	/// </summary>
	void DamageExplosion(const Vector3& collisionPos);

	/// <summary>
	/// �_���[�W����������Ԃ̐F���`�J�`�J�����鏈��
	/// </summary>
	void DamageColorChange();

	/// <summary>
	/// �U�����e:���˂̊J�n���ҋ@����
	/// </summary>
	void AttackTypeRapidFireStartWait();

	/// <summary>
	/// �U�����e:���˂̗��ߏ���
	/// </summary>
	void AttackTypeRapidFireCharge();

	/// <summary>
	/// �U�����e:���˂̔��ˏ���
	/// </summary>
	void AttackTypeRapidFireShot();

	/// <summary>
	/// �U�����e:���˂̖߂�����
	/// </summary>
	void AttackTypeRapidFireReturn();

	/// <summary>
	/// �U�����e:���˂̑ҋ@����
	/// </summary>
	void AttackTypeRapidFireWait();

	/// <summary>
	/// �j��
	/// </summary>
	void Break();


protected: //�ÓI�����o�ϐ�
	//�X�e�[�W�V�[��
	static BaseStageScene* stageScene;
	//�ő�̗�
	static const int maxHP = 6;
	//��C�̃��f��
	static ObjModel* cannonModel;
	//���S��Ԃ̃��f��
	static ObjModel* cannonDeadModel;
	//�G�e�̃��f��
	static ObjModel* bulletModel;
	//�j�󎞂ɏo�����f��
	static std::array<ObjModel*, 5> breakModels;
	//�ʏ�T�C�Y
	static const Vector3 normalSize;
	//�_���[�W��Ԃ̃T�C�Y
	static const Vector3 damageSize;
	//�_���[�W��Ԃ̐F
	static const XMFLOAT4 damageColor;
	//�U�����e:���˂̍s���J��
	static void (GalaxyCannon::* attackTypeRapidFirePhaseFuncTable[])();

protected: //�����o�ϐ�
	//����W
	Vector3 basePos;
	//�̗�
	int HP = maxHP;
	//���S�t���O
	bool isDead = false;
	//��炤�_���[�W��
	int damageNum = 0;
	//�_���[�W�t���O
	bool isDamage = false;
	//�_���[�W���������u�Ԃ�
	bool isDamageTrigger = false;
	//�_���[�W��ԃ^�C�}�[
	int32_t damageTimer = 0;
	//�_���[�W�F��
	bool isDamageColor = false;
	//�e���˃^�C�}�[
	int32_t fireTimer = 0;
	//�e���ˍ��W
	Vector3 bulletShotPos;
	//�U������
	bool isAttack = false;
	//�U���Ŏg���^�C�}�[
	int32_t attackTimer = 0;
	//�U�����e:���˂̍s��
	AttackTypeRapidFirePhase attackTypeRapidFirePhase = AttackTypeRapidFirePhase::StartWait;
	//�U�����e:���˂̊J�n���ҋ@�t���[����
	int rapidFireStartWaitTime = 0;
	//�U�����e:���˂̔��ˉ�
	int rapidFireCount = 0;
};