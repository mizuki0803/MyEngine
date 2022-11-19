#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ステージ選択用惑星
/// </summary>
class StageSelectPlanet : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ステージ選択用惑星</returns>
	static StageSelectPlanet* Create(ObjModel* model, const Vector3& position);

public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
};
