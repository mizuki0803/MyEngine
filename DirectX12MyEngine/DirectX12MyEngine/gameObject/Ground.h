#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �n��
/// </summary>
class Ground : public ObjObject3d
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�n��</returns>
	static Ground* Create(ObjModel* model);
};