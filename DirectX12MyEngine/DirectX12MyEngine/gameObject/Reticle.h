#pragma once
#include "Reticle3D.h"
#include "Reticle2D.h"

/// <summary>
/// 2D���e�B�N���ȈՍ쐬�N���X
/// </summary>
class Reticle
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <returns>���e�B�N��</returns>
	static Reticle* Create(UINT texNumber, float distance, const XMFLOAT2& size);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize(UINT texNumber, float distance, const XMFLOAT2& size);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(const XMMATRIX& matWorld, const XMMATRIX& matView, const XMMATRIX& matProjection);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//getter
	Vector3 GetWorldPos() { return reticle3d->GetWorldPos(); }

private:
	//3D���e�B�N��
	std::unique_ptr<Reticle3D> reticle3d;
	//2D���e�B�N��
	std::unique_ptr<Reticle2D> reticle2d;
};