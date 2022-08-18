#pragma once
#include "BossAvatar.h"

/// <summary>
/// ボス(分身：タイプ04)
/// </summary>
class BossAvatarType04 : public BossAvatar
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス(分身：タイプ04)</returns>
	static BossAvatarType04* Create(ObjModel* model, ObjObject3d* parent, const Vector3& position);

public: //メンバ関数
	/// <summary>
	/// お試し用
	/// </summary>
	void Otamesi() override;
	void Otamesi2() override;
	void Otamesi3() override;
	void Otamesi4() override;
};
