#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 山
/// </summary>
class Mountain : public ObjObject3d
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>山</returns>
	static Mountain* Create(ObjModel* model, const Vector3& position);
};