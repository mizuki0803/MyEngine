#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Vector2.h"

//���@�̑O���錾
class Player;

/// <summary>
/// �Q�[���J����
/// </summary>
class GameCamera : public Camera
{
public: //�ÓI�����o�֐�
	//getter
	static float GetAdvanceSpeed() { return GameCamera::advanceSpeed; }
	static float GetHighSpeedMagnification() { return GameCamera::highSpeedMagnification; }
	static float GetSlowSpeedMagnification() { return GameCamera::slowSpeedMagnification; }

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

	//setter
	void SetIsAdvance(const bool isAdvance) { this->isAdvance = isAdvance; }

protected: //�����o�֐�
	/// <summary>
	/// �J��������
	/// </summary>
	void CameraAction(Player* player);

	/// <summary>
	/// �ė�
	/// </summary>
	virtual void Crash() = 0;

	/// <summary>
	/// ��]
	/// </summary>
	void Rotate(const Vector3& playerRotation);

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move(Player* player);

	/// <summary>
	/// �v���C���[�ɍ��킹�ăm�b�N�o�b�N����
	/// </summary>
	void Knockback(Player* player);

	/// <summary>
	/// �V�F�C�N
	/// </summary>
	void Shake();

	/// <summary>
	/// �X�e�[�W�N���A��s��
	/// </summary>
	virtual void StageClear() = 0;


protected: //�ÓI�����o�ϐ�
	//�X�e�[�W����O�i���鑬��
	static const float advanceSpeed;
	//�n�C�X�s�[�h�ňړ�����Ƃ��̑��x�{��
	static const float highSpeedMagnification;
	//�x���X�s�[�h�ňړ�����Ƃ��̑��x�{��
	static const float slowSpeedMagnification;

protected: //�����o�ϐ�
	//��]
	Vector3 rotation = { 0, 0, 0 };
	//���W
	Vector3 position = { 0, 0, 0 };
	//�J�����̈ړ����E
	Vector2 moveLimitMax = {};
	Vector2 moveLimitMin = {};
	//�O�i���邩
	bool isAdvance = true;
	//�ė���Ԃ�
	bool isCrash = false;
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