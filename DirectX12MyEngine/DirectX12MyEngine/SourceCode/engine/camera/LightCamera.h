#pragma once
#include "Camera.h"

class LightCamera : public Camera
{
public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3& eye, const Vector3& target = { 0, 0, 0 });

	/// <summary>
	/// �ˉe�s����X�V
	/// </summary>
	void UpdateMatProjection() override;
};