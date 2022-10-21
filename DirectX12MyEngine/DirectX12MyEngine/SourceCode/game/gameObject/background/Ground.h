#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ’n–Ê
/// </summary>
class Ground : public ObjObject3d
{
public:
	/// <summary>
	/// ¶¬ˆ—
	/// </summary>
	/// <param name="model">ƒ‚ƒfƒ‹</param>
	/// <returns>’n–Ê</returns>
	static Ground* Create(ObjModel* model);
};