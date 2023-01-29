#pragma once
#include "ObjObject3d.h"

//�X�e�[�W�V�[���̑O���錾
class BaseStageScene;

/// <summary>
/// �M�����N�V�[(�D��)
/// </summary>
class GalaxyBow : public ObjObject3d
{
private: //�U�����e�̃t�F�[�Y
	//�Ή�����
	enum class AttackTypeFlamethrowerPhase {
		Charge,	//����
		Shot,	//����
		Wait,	//�ҋ@
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="parent">�e�I�u�W�F�N�g</param>
	/// <param name="position">���W</param>
	/// <returns>�M�����N�V�[(�D��)</returns>
	static GalaxyBow* Create(ObjObject3d* parent, const Vector3& position);

	//getter
	static const int GetMaxHP() { return GalaxyBow::maxHP; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { GalaxyBow::stageScene = stageScene; }
	static void SetBowModel(ObjModel* model) { GalaxyBow::bowModel = model; }
	static void SetBowSleepModel(ObjModel* model) { GalaxyBow::bowSleepModel = model; }
	static void SetBowDeadModel(ObjModel* model) { GalaxyBow::bowDeadModel = model; }
	static void SetBulletModel(ObjModel* model) { GalaxyBow::bulletModel = model; }

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
	/// �U�����e:�Ή�����
	/// </summary>
	/// <param name="playerPos">���@���W</param>
	void AttackTypeFlamethrower(const Vector3& playerPos);

	/// <summary>
	/// �Q�Ă����Ԃ̃��f������N���Ă����Ԃ̃��f���ɕύX
	/// </summary>
	void ChangeModel();

	//getter	
	const bool GetIsDead() { return isDead; }
	const int GetDamageNum() { return damageNum; }
	const bool GetIsDamageTrigger() { return isDamageTrigger; }
	const bool GetIsAttack() { return isAttack; }

protected: //�����o�֐�
	/// <summary>
	/// �ʏ�e����
	/// </summary>
	/// <param name="scale">�e�̑傫��</param>
	/// <param name="bulletSpeed">�e�̑��x</param>
	void Fire(const float scale, const float bulletSpeed);

	/// <summary>
	/// �W�I�ɔ�΂�����
	/// </summary>
	/// <param name="scale">�e�̑傫��</param>
	/// <param name="bulletSpeed">�e�̑��x</param>
	void RockonFire(const float scale, const float bulletSpeed);

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
	/// <param name="collisionPos">�Փˍ��W</param>
	void DamageExplosion(const Vector3& collisionPos);

	/// <summary>
	/// �_���[�W����������Ԃ̐F���`�J�`�J�����鏈��
	/// </summary>
	void DamageColorChange();

	/// <summary>
	/// �U�����e:�Ή����˂̗��ߏ���
	/// </summary>
	void AttackTypeFlamethrowerCharge();

	/// <summary>
	/// �U�����e:�Ή����˂̔��ˏ���
	/// </summary>
	void AttackTypeFlamethrowerShot();

	/// <summary>
	/// �U�����e:�Ή����˂̑ҋ@����
	/// </summary>
	void AttackTypeFlamethrowerWait();


protected: //�ÓI�����o�ϐ�
	//�X�e�[�W�V�[��
	static BaseStageScene* stageScene;
	//�ő�̗�
	static const int maxHP = 40;
	//�D��̃��f��
	static ObjModel* bowModel;
	//�D��̐Q�Ă����Ԃ̃��f��
	static ObjModel* bowSleepModel;
	//���S��Ԃ̃��f��
	static ObjModel* bowDeadModel;
	//�G�e�̃��f��
	static ObjModel* bulletModel;
	//�ʏ�T�C�Y
	static const Vector3 normalSize;
	//�_���[�W��Ԃ̃T�C�Y
	static const Vector3 damageSize;
	//�_���[�W��Ԃ̐F
	static const XMFLOAT4 damageColor;
	//�U�����e:�Ή����˂̍s���J��
	static void (GalaxyBow::* attackTypeFlamethrowerPhaseFuncTable[])();

protected: //�����o�ϐ�
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
	//�U�����e:�Ή����˂̍s��
	AttackTypeFlamethrowerPhase attackTypeFlamethrowerPhase = AttackTypeFlamethrowerPhase::Charge;
	//�W�I���W
	Vector3 fireTargetPosition;
};