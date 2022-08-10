#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ボス(体)
/// </summary>
class BossBody : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス(体)</returns>
	static BossBody* Create(ObjModel* model, const Vector3& position);

private: //メンバ変数
	//死亡フラグ
	bool isDead = false;
};
