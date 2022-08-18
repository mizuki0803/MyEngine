#pragma once
#include "BossAvatar.h"

/// <summary>
/// ボス(分身：タイプ03)
/// </summary>
class BossAvatarType03 : public BossAvatar
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス(分身：タイプ03)</returns>
	static BossAvatarType03* Create(ObjModel* model, ObjObject3d* parent, const Vector3& position);

public: //メンバ関数
	/// <summary>
	/// お試し用
	/// </summary>
	void Otamesi() override;
	void Otamesi2() override;
	void Otamesi3() override;
	void Otamesi4() override;
};
