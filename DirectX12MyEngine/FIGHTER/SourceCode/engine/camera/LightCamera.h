#pragma once
#include "Camera.h"
#include "Vector2.h"

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

	/// <summary>
	/// �Ǐ]
	/// </summary>
	/// <param name="targetPosition">���W</param>
	void Follow(const Vector3& targetPosition);

	//setter
	void SetProjectionNum(const Vector2& projectionMax, const Vector2& projectionMin) {
		this->projectionMax = projectionMax;
		this->projectionMin = projectionMin;
	}

private: //�����o�ϐ�
	//�v���W�F�N�V�����̍ő�l
	Vector2 projectionMax = { 100, 100 };
	//�v���W�F�N�V�����̍ŏ��l
	Vector2 projectionMin = { -100, -100 };
};