#pragma once
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// �J����
/// </summary>
class Camera
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// �J����������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �J�����A���O�����X�V
	/// </summary>
	void UpdateAngle();

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	void UpdateMatView();

	/// <summary>
	/// �ˉe�s����X�V
	/// </summary>
	void UpdateMatProjection();

	/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveVector(const XMFLOAT3& move);

	/// <summary>
	/// �x�N�g���ɂ�鎋�_�ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveEyeVector(const XMFLOAT3& move);


	//getter
	const XMFLOAT3& GetEye() { return eye; }
	const XMFLOAT3& GetTarget() { return target; }
	const XMMATRIX& GetMatView() { return matView; }
	const XMMATRIX& GetMatProjection() { return matProjection; }
	const XMMATRIX& GetMatBillboard() { return matBillboard; }
	const XMMATRIX& GetMatBillboardY() { return matBillboardY; }
	const XMFLOAT3& GetAngle() { return angle; }
	const float GetDistance() { return distance; }

	//setter
	void SetEye(const XMFLOAT3& eye) { this->eye = eye; dirtyView = true; }
	void SetTarget(const XMFLOAT3& target) { this->target = target; dirtyView = true; }
	void SetAngle(const XMFLOAT3& angle) { this->angle = angle; dirtyAngle = true; }
	void SetDistance(const float distance) { this->distance = distance; dirtyAngle = true; }

private:
	// �r���[�s��
	XMMATRIX matView = XMMatrixIdentity();
	// �ˉe�s��
	XMMATRIX matProjection = XMMatrixIdentity();
	//�r���{�[�h�s��
	XMMATRIX matBillboard = XMMatrixIdentity();
	//Y�����̃r���{�[�h�s��
	XMMATRIX matBillboardY = XMMatrixIdentity();
	// ���_���W
	XMFLOAT3 eye = { 0, 1, -10.0f };
	// �����_���W
	XMFLOAT3 target = { 0, 0, 0 };
	// ������x�N�g��
	XMFLOAT3 up = { 0, 1, 0 };
	//�J�����̉�]�p
	XMFLOAT3 angle = { 0.0f, 0.0f, 0.0f };
	//target����J�����܂ł̋���
	float distance = 10.0f;
	//�_�[�e�B�t���O
	bool dirtyAngle = false;
	bool dirtyView = false;
	bool dirtyProjection = false;
};