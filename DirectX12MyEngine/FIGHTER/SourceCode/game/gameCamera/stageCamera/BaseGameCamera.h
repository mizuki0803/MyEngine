#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Vector2.h"

//���@�̑O���錾
class BasePlayer;

/// <summary>
/// �Q�[���J�������
/// </summary>
class BaseGameCamera : public Camera
{
public: //�ÓI�����o�֐�
	//getter
	static float GetHighSpeedMagnification() { return BaseGameCamera::highSpeedMagnification; }
	static float GetSlowSpeedMagnification() { return BaseGameCamera::slowSpeedMagnification; }

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �V�F�C�N�J�n
	/// </summary>
	void ShakeStart(const float shakePower = 20, const float shakeTime = 30);

	//getter
	const Vector3& GetPosition() { return position; }
	float GetAdvanceSpeed() { return advanceSpeed; }

	//setter
	void SetIsAdvance(const bool isAdvance) { this->isAdvance = isAdvance; }

protected: //�����o�֐�
	/// <summary>
	/// ���[���h�s����X�V
	/// </summary>
	/// <param name="matTrans"></param>
	void UpdateMatWorld(const XMMATRIX& matTrans);

	/// <summary>
	/// ���������Z�������[���h�s����X�V
	/// </summary>
	void UpdateSwayMatWorld(const XMMATRIX& matTrans);

	/// <summary>
	/// ���_�ƒ����_���X�V
	/// </summary>
	void UpdateEyeTarget();

	/// <summary>
	/// �J��������
	/// </summary>
	void CameraAction(BasePlayer* player);

	/// <summary>
	/// �ė�
	/// </summary>
	virtual void Crash() = 0;

	/// <summary>
	/// ��]
	/// </summary>
	void Rotate(const Vector3& playerRotation);

	/// <summary>
	/// �J����������]
	/// </summary>
	void Sway();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move(BasePlayer* player);

	/// <summary>
	/// �v���C���[�ɍ��킹�ăm�b�N�o�b�N����
	/// </summary>
	void Knockback(BasePlayer* player);

	/// <summary>
	/// �V�F�C�N
	/// </summary>
	void Shake();

	/// <summary>
	/// �X�e�[�W�N���A��s��
	/// </summary>
	virtual void StageClear() = 0;


protected: //�ÓI�����o�ϐ�
	//�n�C�X�s�[�h�ňړ�����Ƃ��̑��x�{��
	static const float highSpeedMagnification;
	//�x���X�s�[�h�ňړ�����Ƃ��̑��x�{��
	static const float slowSpeedMagnification;

protected: //�����o�ϐ�
	//��]
	Vector3 rotation = { 0, 0, 0 };
	//���W
	Vector3 position = { 0, 0, 0 };
	//���ۂɃJ�����̎��_,�����_,��x�N�g���Ɏg�p���郏�[���h�ϊ��s��
	XMMATRIX cameraMatWorld = {};
	//�J�����̈ړ����E
	Vector2 moveLimitMax = {};
	Vector2 moveLimitMin = {};
	//�O�i���邩
	bool isAdvance = true;
	//�O�i�̃X�s�[�h
	float advanceSpeed = 0;
	//�ė���Ԃ�
	bool isCrash = false;
	//x��������]�����]��
	bool isSwayXUp = true;
	//x��������]�p
	float swayX = 0.0f;
	//x�������̑��x
	float swayXSpeed = 0;
	//x�������̑��x���ő傩
	bool isSwaySpeedMax = false;
	//�V�F�C�N���邩
	bool isShake = false;
	//�J�����V�F�C�N�p�^�C�}�[
	int32_t shakeTimer = 0;
	//�V�F�C�N���鎞��
	float shakeTime = 0;
	//�V�F�C�N�ő�̋���
	float maxShakePower = 0;
	//�X�e�[�W�N���A��̓��������邩
	bool isStageClearMode = false;
};