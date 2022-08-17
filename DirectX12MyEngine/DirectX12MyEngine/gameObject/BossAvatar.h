#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �{�X(���g)
/// </summary>
class BossAvatar : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X(���g)</returns>
	static BossAvatar* Create(ObjModel* model, ObjObject3d* parent);

private: //�ÓI�����o�ϐ�
	//�̗�
	static const int maxHP = 20;

private: //�����o�ϐ�
	//�̗�
	int HP = maxHP;
	//���S�t���O
	bool isDead = false;
};
