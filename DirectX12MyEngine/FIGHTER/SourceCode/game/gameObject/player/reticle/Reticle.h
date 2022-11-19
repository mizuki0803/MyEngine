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
	static Reticle* Create(UINT texNumber, float distance, const Vector2& size);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize(UINT texNumber, float distance, const Vector2& size);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(const XMMATRIX& matWorld, const XMMATRIX& matView, const XMMATRIX& matProjection);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//getter
	Reticle3D* GetReticle3D() { return reticle3d.get(); }
	Reticle2D* GetReticle2D() { return reticle2d.get(); }

private:
	//3D���e�B�N��
	std::unique_ptr<Reticle3D> reticle3d;
	//2D���e�B�N��
	std::unique_ptr<Reticle2D> reticle2d;
};