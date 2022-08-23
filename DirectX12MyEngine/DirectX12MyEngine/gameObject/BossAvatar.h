#pragma once
#include "ObjObject3d.h"

//GameScene�̑O���錾
class GameScene;

/// <summary>
/// �{�X(���g)
/// </summary>
class BossAvatar : public ObjObject3d
{
public: //�ÓI�����o�֐�
	//getter
	static const int GetMaxHP() { return BossAvatar::maxHP; }

	//setter
	static void SetGameScene(GameScene* gameScene) { BossAvatar::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { BossAvatar::bulletModel = model; }

public: //�����o�֐�
	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	/// <param name="damageNum">�_���[�W��</param>
	void Damage(int damageNum);

	/// <summary>
	/// �U�����eB
	/// </summary>
	void AttackTypeB();

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
	/// �Œ�ʒu�ɖ߂�Ƃ��̏o�����W���L�^
	/// </summary>
	void SetReturnStartPos();

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }

protected:
	/// <summary>
	/// �e����
	/// </summary>
	void Fire();

protected: //�ÓI�����o�ϐ�
	//�Q�[���V�[��
	static GameScene* gameScene;
	//�G�e�̃��f��
	static ObjModel* bulletModel;
	//�̗�
	static const int maxHP = 20;
	//�U����Ԃ�Y����]
	static const float attackModeRotY;
	//�ҋ@��Ԃ�Y����]
	static const float waitModeRotY;

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
};
