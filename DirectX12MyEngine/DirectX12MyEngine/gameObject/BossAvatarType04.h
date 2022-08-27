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
	/// 攻撃内容Bのロックオン処理
	/// </summary>
	void AttackTypeBLockon() override;

	/// <summary>
	/// 攻撃内容Cの中心に移動させる処理
	/// </summary>
	void AttackTypeCMoveCenter() override;

	/// <summary>
	/// 攻撃内容Dの待機処理
	/// </summary>
	void AttackTypeDWait() override;

	/// <summary>
	/// 攻撃内容Dの移動させる処理
	/// </summary>
	void AttackTypeDMove() override;
};
