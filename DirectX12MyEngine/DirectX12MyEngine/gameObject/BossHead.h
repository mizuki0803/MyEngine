#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �{�X(��)
/// </summary>
class BossHead : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X(��)</returns>
	static BossHead* Create(ObjModel* model, ObjObject3d* parent);

private: //�����o�ϐ�
	//���S�t���O
	bool isDead = false;
};
