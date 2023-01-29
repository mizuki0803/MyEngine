#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �r��
/// </summary>
class Building : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <returns>�r��</returns>
	static Building* Create(ObjModel* model, const Vector3& position);

public: //�����o�֐�
	/// <summary>
	/// ��ʎ�O�܂ōs������폜���鏈��
	/// </summary>
	/// <param name="pos">���W</param>
	void FrontOfScreenDelete(const Vector3& pos);

	//getter
	bool GetIsDead() { return isDead; }

private: //�����o�ϐ�
	//���S�t���O
	bool isDead = false;
};
