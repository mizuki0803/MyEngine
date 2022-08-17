#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ボス(本体)
/// </summary>
class BossMainBody : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス(本体)</returns>
	static BossMainBody* Create(ObjModel* model, const Vector3& position);

private: //メンバ変数
	//死亡フラグ
	bool isDead = false;
};
