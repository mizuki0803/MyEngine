#pragma once
#include <DirectXMath.h>
#include "Camera.h"

//�^�C�g���V�[���p���@�N���X��O���錾
class TitlePlayer;

/// <summary>
/// �^�C�g���V�[���p�J����
/// </summary>
class TitleCamera : public Camera
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
	/// �o���J�n
	/// </summary>
	void SortieStart();

	//setter
	void SetTitlePlayer(TitlePlayer* player) { this->player = player; }

private: //�����o�֐�
	/// <summary>
	/// �o��
	/// </summary>
	void Sortie();

private: //�����o�ϐ�
	//�v���C���[���@
	TitlePlayer* player = nullptr;
	//�^�[�Q�b�g�Ǝ��_�̋���
	Vector3 eyeDistance;
	//��]�p�x
	float rotAngle = 0;
	//�o���J�n����̊J�n�p�x
	float beforeRoundRotAngle = 0;
	//�o���J�n����p�^�C�}�[
	int32_t rotRoundTimer = 0;
};