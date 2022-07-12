#pragma once
#include "ObjObject3d.h"

class Skydome : public ObjObject3d 
{
public:
	/// <summary>
	/// ¶¬ˆ—
	/// </summary>
	/// <param name="model">ƒ‚ƒfƒ‹</param>
	/// <returns>“V‹…</returns>
	static Skydome* Create(ObjModel* model);
};