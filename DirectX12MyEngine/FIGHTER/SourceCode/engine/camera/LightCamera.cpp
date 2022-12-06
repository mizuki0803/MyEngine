#include "LightCamera.h"
#include "WindowApp.h"

void LightCamera::Initialize(const Vector3& eye, const Vector3& target)
{
	//�J����������
	Camera::Initialize();

	//���_�A�����_���Z�b�g
	this->eye = eye;
	this->target = target;
	up = { 1, 0, 0 };
}

void LightCamera::UpdateMatProjection()
{
	matProjection = XMMatrixOrthographicOffCenterLH(
		-120, 120,
		-120, 120,
		0.1f, 2000.0f
	);
}

void LightCamera::Follow(const Vector3& targetPosition)
{
	eye = { targetPosition.x, 500, targetPosition.z };
	target = { targetPosition.x, 0, targetPosition.z };

	dirtyView = true;
	dirtyProjection = true;
}
