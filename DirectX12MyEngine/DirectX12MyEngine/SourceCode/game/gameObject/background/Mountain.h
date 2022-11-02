#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 山
/// </summary>
class Mountain : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>山</returns>
	static Mountain* Create(ObjModel* model, const Vector3& position);

public: //メンバ関数
	/// <summary>
	/// 画面手前まで行ったら削除する処理
	/// </summary>
	void FrontOfScreenDelete(const Vector3& pos);

	//getter
	bool GetIsDead() { return isDead; }

private: //メンバ変数
	//死亡フラグ
	bool isDead = false;
};
