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
	enum class AttackTypeAPhase {
		Lockon,
		Shot,
		Stay,
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X(�{��)</returns>
	static BossMainBody* Create(ObjModel* model, const Vector3& position);

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
	/// �U�����eA
	/// </summary>
	void AttackTypeA(const Vector3& playerPosition);

	/// <summary>
	/// �U�����eB
	/// </summary>
	void AttackTypeB();

	/// <summary>
	/// �U�����eC
	/// </summary>
	void AttackTypeC(const Vector3& playerPosition);

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

private: //�����o�֐�
	/// <summary>
	/// �e����
	/// </summary>
	void Fire(const float scale, const float bulletSpeed);

	/// <summary>
	/// �U�����eA�̃��b�N�I������
	/// </summary>
	void AttackTypeALockon();

	/// <summary>
	/// �U�����eA�̒e���ˏ���
	/// </summary>
	void AttackTypeAShot();

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
	//�U�����eA�̍s���J��
	static void (BossMainBody::* attackTypeAPhaseFuncTable[])();

private: //�����o�ϐ�
	//�������W
	Vector3 bornPos;
	//�̗�
	int HP = maxHP;
	//���S�t���O
	bool isDead = false;
	//�e���˃^�C�}�[
	int32_t fireTimer = 0;
	//�Œ�ʒu�ɖ߂�Ƃ��̏o�����W
	Vector3 returnStartPos;
	//�U�����eA�̍s��
	AttackTypeAPhase attackAPhase = AttackTypeAPhase::Lockon;
	//�U�����eA�Ŏg���^�C�}�[
	int32_t attackATimer = 0;
	//�U�����eB�Ŏg���^�C�}�[
	int32_t attackBTimer = 0;
};
