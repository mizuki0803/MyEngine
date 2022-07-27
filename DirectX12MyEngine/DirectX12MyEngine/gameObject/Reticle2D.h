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
	/// <param name="model">���f��</param>
	/// <returns>���e�B�N��2D</returns>
	static Reticle2D* Create(UINT texNumber, const Vector2& size);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize(UINT texNumber, Vector2 anchorpoint, bool isFlipX, bool isFlipY) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �G���b�N�I��
	/// </summary>
	/// <param name="enemy">���b�N�I���Ώۂ̓G</param>
	void LockonEnemy(Enemy* enemy);

	/// <summary>
	/// �G���b�N�I������
	/// </summary>
	void UnlockonEnemy();


	//getter
	const bool GetIsLockon() { return isLockon; }
	Enemy* GetLockonEnemy() { return this->lockonEnemy; }

	//setter
	void SetWorldPos3d(const Vector3& worldPos3d) { this->worldPos3d = worldPos3d; }
	void SetMatViewProjection(const XMMATRIX& matView, const XMMATRIX& matProjection) {
		this->matView = matView;
		this->matProjection = matProjection;
	}

private: //�����o�ϐ�
	/// <summary>
	/// ���b�N�I�����̈ړ�
	/// </summary>
	void MoveLockon();

	/// <summary>
	/// ���b�N�I���ł͂Ȃ��Ƃ��̈ړ�
	/// </summary>
	void MoveNotLockon();

private: //�ÓI�����o�ϐ�
	//�ʏ펞�̐F
	static const XMFLOAT4 normalColor;
	//���b�N�I�����̐F
	static const XMFLOAT4 lockonColor;

private:
	//3D���e�B�N���̃��[���h���W
	Vector3 worldPos3d = {};
	//�r���[�s��
	XMMATRIX matView = {};
	//�v���W�F�N�V�����s��
	XMMATRIX matProjection = {};
	//�G
	Enemy* lockonEnemy = nullptr;
	//���b�N�I�����Ă��邩
	bool isLockon = false;
	//���e�B�N�������̈ʒu(���@�Ǐ])�ɖ߂���
	bool isBackPos = false;
	//���e�B�N�������̈ʒu(���@�Ǐ])�ɖ߂��^�C�}�[
	float backTimer = 0;
};
