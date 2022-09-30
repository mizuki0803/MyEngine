#pragma once
#include "Reticle2D.h"
#include "Enemy.h"

/// <summary>
/// ���@�t����2D���e�B�N��(��)
/// </summary>
class PlayerFarReticle2D : public Reticle2D
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>���@�t����2D���e�B�N��(��)</returns>
	static PlayerFarReticle2D* Create(UINT texNumber, const Vector2& size);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`���[�W��Ԃ��J�n����
	/// </summary>
	void ChargeModeStart();

	/// <summary>
	/// �`���[�W��Ԃ��I������
	/// </summary>
	void ChargeModeEnd();

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
	const bool GetIsChargeMode() { return isChargeMode; }
	const bool GetIsLockon() { return isLockon; }
	Enemy* GetLockonEnemy() { return lockonEnemy; }

	//setter
	void SetWorldPos3d(const Vector3& worldPos3d) { this->worldPos3d = worldPos3d; }
	void SetMatViewProjection(const XMMATRIX& matView, const XMMATRIX& matProjection) {
		this->matView = matView;
		this->matProjection = matProjection;
	}

private: //�����o�ϐ�
	/// <summary>
	/// �`���[�W��Ԃ̋���
	/// </summary>
	void ChargeMode();

	/// <summary>
	/// �`���[�W��ԊJ�n��Ԃ̑傫���ύX����
	/// </summary>
	void ChargeModeStartSizeChange();

	/// <summary>
	/// �`���[�W��ԏI����Ԃ̑傫���ύX����
	/// </summary>
	void ChargeModeEndSizeChange();

	/// <summary>
	/// �`���[�W��Ԃ̒ʏ펞�傫���ύX����
	/// </summary>
	void ChargeModeSizeChange();

	/// <summary>
	/// ���b�N�I�����̈ړ�
	/// </summary>
	void MoveLockon();

	/// <summary>
	/// �ʏ�ړ�
	/// </summary>
	void Move() override;


private: //�ÓI�����o�ϐ�
	//���b�N�I�����̐F
	static const XMFLOAT4 chargeLockonColor;

private:
	//�ʏ펞�̑傫��
	Vector2 normalSize;
	//�`���[�W��Ԃ�
	bool isChargeMode = false;
	//�`���[�W��ԊJ�n��Ԃ�
	bool isChargeModeStart = false;
	//�`���[�W��ԏI����Ԃ�
	bool isChargeModeEnd = false;
	//�`���[�W��ԗp�^�C�}�[
	int32_t chargeTimer = 0;
	//�ύX�O�̑傫���L�^�p
	Vector2 beforeChangeSize;
	//���b�N�I���G
	Enemy* lockonEnemy = nullptr;
	//���b�N�I�����Ă��邩
	bool isLockon = false;
	//���e�B�N�������̈ʒu(���@�Ǐ])�ɖ߂���
	bool isBackPos = false;
	//���e�B�N�������̈ʒu(���@�Ǐ])�ɖ߂��^�C�}�[
	float backTimer = 0;
};
