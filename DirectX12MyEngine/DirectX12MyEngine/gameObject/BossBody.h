#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �{�X(��)
/// </summary>
class BossBody : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X(��)</returns>
	static BossBody* Create(ObjModel* model, const Vector3& position);

private: //�����o�ϐ�
	//���S�t���O
	bool isDead = false;
};
