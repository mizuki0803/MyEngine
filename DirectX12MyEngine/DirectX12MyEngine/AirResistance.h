#pragma once
#include "Object3d.h"

class AirResistance 
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// ��C��R������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>��C��R��</returns>
	static AirResistance* Create(Model* model = nullptr);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~AirResistance();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>����</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//getter
	XMFLOAT3 GetPosition() { return sphereObject->GetPosition(); }

private:
	/// <summary>
	/// �C��
	/// </summary>
	void Turret();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();


private:
	//���I�u�W�F�N�g
	Object3d* sphereObject = nullptr;
	//�����ʒu
	const XMFLOAT3 initPos = { -900, 0, 0 };
	//�������x
	const XMFLOAT2 initVel = { 13, 13 };
	//���x
	XMFLOAT2 vel = {};
	//�����x
	XMFLOAT2 accel = {};
	//����(kg)
	const float mass = 1;
	//�n�ʂɂ�����
	bool isOnGround = false;
};