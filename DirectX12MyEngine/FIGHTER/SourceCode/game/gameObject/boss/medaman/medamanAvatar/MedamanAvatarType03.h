#pragma once
#include "MedamanAvatar.h"

/// <summary>
/// メダマーン(分身：タイプ03)
/// </summary>
class MedamanAvatarType03 : public MedamanAvatar
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>メダマーン(分身：タイプ03)</returns>
	static MedamanAvatarType03* Create(ObjObject3d* parent, const Vector3& position);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

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

private: //メンバ関数
	/// <summary>
	/// 死亡したときの動き
	/// </summary>
	void DeadAction() override;
};
