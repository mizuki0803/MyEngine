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

private: //�����o�ϐ�
	//���S�t���O
	bool isDead = false;
};
