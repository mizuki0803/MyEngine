#pragma once
#include <DirectXMath.h>
#include "Camera.h"

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera : public Camera
{
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:
	//回転
	Vector3 rotation = { 0, 0, 0 };
	//座標
	Vector3 position = { 0, 0, 0 };
};