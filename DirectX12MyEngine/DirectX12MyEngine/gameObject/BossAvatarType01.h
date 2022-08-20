#pragma once
#include "BossAvatar.h"

/// <summary>
/// ボス(分身：タイプ01)
/// </summary>
class BossAvatarType01 : public BossAvatar
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス(分身：タイプ01)</returns>
	static BossAvatarType01* Create(ObjModel* model, ObjObject3d* parent, const Vector3& position);
};
