#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �{�X(���g)
/// </summary>
class BossAvatar : public ObjObject3d
{
public: //�����o�֐�
	/// <summary>
	/// �������p
	/// </summary>
	virtual void Otamesi() = 0;
	virtual void Otamesi2() = 0;
	virtual void Otamesi3() = 0;
	virtual void Otamesi4() = 0;

	//getter
	const bool GetIsDead() { return isDead; }

protected: //�ÓI�����o�ϐ�
	//�̗�
	static const int maxHP = 20;
	//�ړ����x
	static const float moveSpeed;

protected: //�����o�ϐ�
	//�̗�
	int HP = maxHP;
	//���S�t���O
	bool isDead = false;
};
