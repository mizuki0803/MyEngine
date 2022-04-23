#pragma once
#include "Object3d.h"

class FrictionSphere
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
	/// ���C������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>���C��</returns>
	static FrictionSphere* Create(Model* model = nullptr);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FrictionSphere();

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
	/// ���C��������
	/// </summary>
	void Friction();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();


private:
	//���I�u�W�F�N�g
	Object3d* sphereObject = nullptr;
	//�����ʒu
	const XMFLOAT3 initPos = { -800, 0, 0 };
	//�������x
	const float initVel = 10;
	//���x
	float vel = 0;
	//����(kg)
	const float mass = 100;
	//���C�W��
	const float frictionCoefficient = 0.5f;
};