#pragma once
#include "Object3d.h"

/// <summary>
/// �C��
/// </summary>
class Turret
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
	/// �C����쐬
	/// </summary>
	/// <param name="model">�g�p���郂�f��</param>
	/// <returns>�C��</returns>
	static Turret* Create(Model* turretModel, Model* bulletModel);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Turret();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">�g�p���郂�f��</param>
	/// <returns>����</returns>
	bool Initialize(Model* turretModel, Model* bulletModel);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//getter
	XMFLOAT3 GetBulletPosition() { return bulletObject->GetPosition(); };

private:
	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

private:
	//�C��I�u�W�F�N�g
	Object3d* turretObject = nullptr;
	//�e�I�u�W�F�N�g
	Object3d* bulletObject = nullptr;
	//���˒n�_
	const XMFLOAT3 startPos = { -1000, 10, 0 };
	//�d��
	const float gravity = 9.8f / 60;
	//���x
	XMFLOAT2 vel = { 10, -10 };
	//�n�ʂɒ�������
	bool isOnGround = false;
};