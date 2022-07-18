#pragma once
#include "Sprite.h"
#include "Reticle3D.h"

/// <summary>
/// 2D���e�B�N��
/// </summary>
class Reticle2D : public Sprite
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>���e�B�N��2D</returns>
	static Reticle2D* Create(UINT texNumber, const XMFLOAT2& size);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	//void Draw() override;

	//setter
	void SetWorldPos3d(const Vector3& worldPos3d) { this->worldPos3d = worldPos3d; }
	void SetMatViewProjection(const XMMATRIX& matView, const XMMATRIX& matProjection) {
		this->matView = matView;
		this->matProjection = matProjection;
	}

private:
	//3D���e�B�N���̃��[���h���W
	Vector3 worldPos3d = {};
	//�r���[�s��
	XMMATRIX matView = {};
	//�v���W�F�N�V�����s��
	XMMATRIX matProjection = {};
};
