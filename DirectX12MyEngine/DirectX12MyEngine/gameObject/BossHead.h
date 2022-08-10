#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ボス(頭)
/// </summary>
class BossHead : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス(頭)</returns>
	static BossHead* Create(ObjModel* model, ObjObject3d* parent);

private: //メンバ変数
	//死亡フラグ
	bool isDead = false;
};
