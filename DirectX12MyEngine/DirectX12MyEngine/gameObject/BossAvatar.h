#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ボス(分身)
/// </summary>
class BossAvatar : public ObjObject3d
{
public: //静的メンバ関数
	//getter
	static const int GetMaxHP() { return BossAvatar::maxHP; }

public: //メンバ関数
	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="damageNum">ダメージ量</param>
	void Damage(int damageNum);

	/// <summary>
	/// お試し用
	/// </summary>
	virtual void Otamesi() = 0;
	virtual void Otamesi2() = 0;
	virtual void Otamesi3() = 0;
	virtual void Otamesi4() = 0;

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }

protected: //静的メンバ変数
	//体力
	static const int maxHP = 20;
	//移動速度
	static const float moveSpeed;

protected: //メンバ変数
	//体力
	int HP = maxHP;
	//死亡フラグ
	bool isDead = false;
};
