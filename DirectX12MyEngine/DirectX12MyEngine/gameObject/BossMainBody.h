#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �{�X(�{��)
/// </summary>
class BossMainBody : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X(�{��)</returns>
	static BossMainBody* Create(ObjModel* model, const Vector3& position);

	//getter
	static const int GetMaxHP() { return BossMainBody::maxHP; }

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
	static const int maxHP = 60;
	//�U����Ԃ�Y����]
	static const float attackModeRotY;
	//�ҋ@��Ԃ�Y����]
	static const float waitModeRotY;

private: //�����o�ϐ�
	//�������W
	Vector3 startPos;
	//�̗�
	int HP = maxHP;
	//���S�t���O
	bool isDead = false;
};
