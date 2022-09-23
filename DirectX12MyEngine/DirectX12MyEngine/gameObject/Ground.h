#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 地面
/// </summary>
class Ground : public ObjObject3d
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>地面</returns>
	static Ground* Create(ObjModel* model);
};