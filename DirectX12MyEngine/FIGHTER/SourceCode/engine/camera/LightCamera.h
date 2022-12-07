#pragma once
#include "Camera.h"
#include "Vector2.h"

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

	//setter
	void SetProjectionNum(const Vector2& projectionMax, const Vector2& projectionMin) {
		this->projectionMax = projectionMax;
		this->projectionMin = projectionMin;
	}

private: //メンバ変数
	//プロジェクションの最大値
	Vector2 projectionMax = { 100, 100 };
	//プロジェクションの最小値
	Vector2 projectionMin = { -100, -100 };
};