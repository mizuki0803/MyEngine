#pragma once
#include "Camera.h"

class LightCamera : public Camera
{
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& eye, const Vector3& target = { 0, 0, 0 });

	/// <summary>
	/// 射影行列を更新
	/// </summary>
	void UpdateMatProjection() override;

	/// <summary>
	/// 追従
	/// </summary>
	/// <param name="targetPosition">座標</param>
	void Follow(const Vector3& targetPosition);
};