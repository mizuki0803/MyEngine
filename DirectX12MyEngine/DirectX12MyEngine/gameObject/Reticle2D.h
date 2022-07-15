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
	static Reticle2D* Create(UINT texNumber);

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
	void SetReticle(Reticle3D* reticle) { this->reticle = reticle; }
	void SetMatViewProjection(const XMMATRIX& matView, const XMMATRIX& matProjection) {
		this->matView = matView;
		this->matProjection = matProjection;
	}

private:
	//3D���e�B�N��
	Reticle3D* reticle = nullptr;
	//�r���[�s��
	XMMATRIX matView = {};
	//�v���W�F�N�V�����s��
	XMMATRIX matProjection = {};
};
