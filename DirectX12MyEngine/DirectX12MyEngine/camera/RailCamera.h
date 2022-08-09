#pragma once
#include <DirectXMath.h>
#include "Camera.h"

//���@�N���X��O���錾
class Player;

/// <summary>
/// ���[���J����
/// </summary>
class RailCamera : public Camera
{
public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �V�F�C�N�J�n
	/// </summary>
	void ShakeStart();

	//setter
	void SetPlayer(Player* player) { this->player = player; }
	void SetIsAdvance(const bool isAdvance) { this->isAdvance = isAdvance; }

private: //�����o�֐�
	/// <summary>
	/// ��]
	/// </summary>
	void Rotate();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �v���C���[�ɍ��킹�ăm�b�N�o�b�N����
	/// </summary>
	void Knockback();

	/// <summary>
	/// �V�F�C�N
	/// </summary>
	void Shake();

private: //�����o�ϐ�
	//�v���C���[���@
	Player* player = nullptr;
	//��]
	Vector3 rotation = { 0, 0, 0 };
	//���W
	Vector3 position = { 0, 0, 0 };
	//�O�i���邩
	bool isAdvance = true;
	//�V�F�C�N���邩
	bool isShake = false;
	//�J�����V�F�C�N�p�^�C�}�[
	int32_t shakeTimer = 0;
};