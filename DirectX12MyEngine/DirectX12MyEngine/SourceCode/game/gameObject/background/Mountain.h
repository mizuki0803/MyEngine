#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �R
/// </summary>
class Mountain : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�R</returns>
	static Mountain* Create(ObjModel* model, const Vector3& position);

public: //�����o�֐�
	/// <summary>
	/// ��ʎ�O�܂ōs������폜���鏈��
	/// </summary>
	void FrontOfScreenDelete(const Vector3& pos);

	//getter
	bool GetIsDead() { return isDead; }

private: //�����o�ϐ�
	//���S�t���O
	bool isDead = false;
};
