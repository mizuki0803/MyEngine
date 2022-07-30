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
public: //�ÓI�����o�֐�
	/// <summary>
	/// ���@���Z�b�g
	/// </summary>
	/// <param name="player">���@</param>
	static void SetPlayer(Player* player) { RailCamera::player = player; }


public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	/// ��]
	/// </summary>
	void Rotate();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

private: //�ÓI�����o�ϐ�
	//�v���C���[���@
	static Player* player;

private: //�����o�ϐ�
	//��]
	Vector3 rotation = { 0, 0, 0 };
	//���W
	Vector3 position = { 0, 0, 0 };
};