#pragma once
#include "Reticle3D.h"
#include "PlayerFarReticle2D.h"

/// <summary>
/// ���@�t����2D���e�B�N��(��)�ȈՍ쐬�N���X
/// </summary>
class PlayerFarReticle
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <returns>���@�t����2D���e�B�N��(��)�ȈՍ쐬�N���X</returns>
	static PlayerFarReticle* Create(UINT texNumber, float distance, const Vector2& normalSize, const Vector2& chargeModeSize);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize(UINT texNumber, float distance, const Vector2& normalSize, const Vector2& chargeModeSize);

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
	PlayerFarReticle2D* GetReticle2D() { return reticle2d.get(); }

private:
	//3D���e�B�N��
	std::unique_ptr<Reticle3D> reticle3d;
	//���@�t����2D���e�B�N��(��)
	std::unique_ptr<PlayerFarReticle2D> reticle2d;
};