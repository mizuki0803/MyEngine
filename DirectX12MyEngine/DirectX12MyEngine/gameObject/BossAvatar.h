#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ボス(分身)
/// </summary>
class BossAvatar : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス(分身)</returns>
	static BossAvatar* Create(ObjModel* model, ObjObject3d* parent);

private: //静的メンバ変数
	//体力
	static const int maxHP = 20;

private: //メンバ変数
	//体力
	int HP = maxHP;
	//死亡フラグ
	bool isDead = false;
};
