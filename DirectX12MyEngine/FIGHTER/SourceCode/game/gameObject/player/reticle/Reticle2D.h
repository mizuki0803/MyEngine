#pragma once
#include "Sprite.h"
#include "Reticle3D.h"
#include "Enemy.h"

/// <summary>
/// 2D���e�B�N��
/// </summary>
class Reticle2D : public Sprite
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="size">�傫��</param>
	/// <returns>���e�B�N��2D</returns>
	static Reticle2D* Create(const Texture& texture, const Vector2& size);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="isFlipX">���E���肷�邩</param>
	/// <param name="isFlipY">�㉺���]���邩</param>
	/// <returns>����</returns>
	bool Initialize(const Texture& texture, const Vector2& anchorpoint, bool isFlipX, bool isFlipY) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	//setter
	void SetWorldPos3d(const Vector3& worldPos3d) { this->worldPos3d = worldPos3d; }
	void SetMatViewProjection(const XMMATRIX& matView, const XMMATRIX& matProjection) {
		this->matView = matView;
		this->matProjection = matProjection;
	}

protected: //�����o�ϐ�
	/// <summary>
	/// �ʏ�ړ�
	/// </summary>
	virtual void Move();

protected: //�ÓI�����o�ϐ�
	//�ʏ펞�̐F
	static const XMFLOAT4 normalColor;

protected:
	//3D���e�B�N���̃��[���h���W
	Vector3 worldPos3d = {};
	//�r���[�s��
	XMMATRIX matView = {};
	//�v���W�F�N�V�����s��
	XMMATRIX matProjection = {};
};
