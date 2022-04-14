#pragma once
#include "Object3d.h"

/// <summary>
/// ������
/// </summary>
class FallSphere 
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
	/// ���������쐬
	/// </summary>
	/// <param name="model">�g�p���郂�f��</param>
	/// <returns>������</returns>
	static FallSphere* Create(Model* model);

public:	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FallSphere();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">�g�p���郂�f��</param>
	/// <returns>����</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	
	//getter
	XMFLOAT3 GetPosition() { return fallSphereObject->GetPosition(); };

private:
	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

private:
	//�������I�u�W�F�N�g
	Object3d* fallSphereObject = nullptr;
	//�����n�_
	const XMFLOAT3 startPos = { 0, 1900, 0 };
	//�d��
	const float gravity = 9.8f / 60;
	//���x
	float velY = 0;
	//�n�ʂɒ�������
	bool isOnGround = false;
};