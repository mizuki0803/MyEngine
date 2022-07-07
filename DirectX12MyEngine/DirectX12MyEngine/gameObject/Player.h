#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 自機
/// </summary>
class Player : public ObjObject3d
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>自機</returns>
	static Player* Create(ObjModel* model);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;


private: //メンバ関数
	/// <summary>
	/// 移動
	/// </summary>
	void Move();

private:
};