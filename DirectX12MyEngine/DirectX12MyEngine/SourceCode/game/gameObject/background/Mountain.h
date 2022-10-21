#pragma once
#include "ObjObject3d.h"

/// <summary>
/// R
/// </summary>
class Mountain : public ObjObject3d
{
public:
	/// <summary>
	/// ¶¬ˆ—
	/// </summary>
	/// <param name="model">ƒ‚ƒfƒ‹</param>
	/// <returns>R</returns>
	static Mountain* Create(ObjModel* model, const Vector3& position);
};