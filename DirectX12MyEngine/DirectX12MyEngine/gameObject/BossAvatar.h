#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �{�X(���g)
/// </summary>
class BossAvatar : public ObjObject3d
{
public: //�ÓI�����o�֐�
	//getter
	static const int GetMaxHP() { return BossAvatar::maxHP; }

public: //�����o�֐�
	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	/// <param name="damageNum">�_���[�W��</param>
	void Damage(int damageNum);

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

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }

protected: //�ÓI�����o�ϐ�
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
};
