#pragma once
#include "Reticle3D.h"
#include "PlayerNearReticle2D.h"

/// <summary>
/// ���@�t����2D���e�B�N��(��)�ȈՍ쐬�N���X
/// </summary>
class PlayerNearReticle
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="distance">���ˑΏۂ���̋���</param>
	/// <param name="size">�傫��</param>
	/// <returns>���@�t����2D���e�B�N��(��)�ȈՍ쐬�N���X</returns>
	static PlayerNearReticle* Create(const Texture& texture, float distance, const Vector2& size);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="distance">���ˑΏۂ���̋���</param>
	/// <param name="size">�傫��</param>
	/// <returns>����</returns>
	bool Initialize(const Texture& texture, float distance, const Vector2& size);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="matWorld">���[���h�s��</param>
	/// <param name="matView">�r���[�s��</param>
	/// <param name="matProjection">�v���W�F�N�V�����s��</param>
	void Update(const XMMATRIX& matWorld, const XMMATRIX& matView, const XMMATRIX& matProjection);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//getter
	Reticle3D* GetReticle3D() { return reticle3d.get(); }
	PlayerNearReticle2D* GetReticle2D() { return reticle2d.get(); }

private:
	//3D���e�B�N��
	std::unique_ptr<Reticle3D> reticle3d;
	//���@�t����2D���e�B�N��(��)
	std::unique_ptr<PlayerNearReticle2D> reticle2d;
};