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
	/// <returns>ボス(分身：タイプ04)</returns>
	static BossAvatarType04* Create(ObjObject3d* parent, const Vector3& position);

public: //メンバ関数
	/// <summary>
	/// 攻撃内容:分身体当たりのロックオン処理
	/// </summary>
	void AttackTypeAvatarBodyBlowLockon() override;

	/// <summary>
	/// 攻撃内容:分身ガトリング砲の中心に移動させる処理
	/// </summary>
	void AttackTypeAvatarGatlingMoveCenter() override;

	/// <summary>
	/// 攻撃内容:分身巨大弾の待機処理
	/// </summary>
	void AttackTypeAvatarGiantBulletWait() override;

	/// <summary>
	/// 攻撃内容:分身巨大弾の移動させる処理
	/// </summary>
	void AttackTypeAvatarGiantBulletMove() override;
};
