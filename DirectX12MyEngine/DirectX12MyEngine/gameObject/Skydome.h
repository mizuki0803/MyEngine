#pragma once
#include "ObjObject3d.h"

class Skydome : public ObjObject3d 
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�V��</returns>
	static Skydome* Create(ObjModel* model);
};