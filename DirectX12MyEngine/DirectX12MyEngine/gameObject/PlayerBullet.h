#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ���@�e
/// </summary>
class PlayerBullet : public ObjObject3d
{
public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	void OnCollision();

	//getter
	Vector3 GetWorldPos();
	bool GetIsDead() const { return isDead; }

protected: //�����o�ϐ�
	//���x
	Vector3 velocity;
	//���S�t���O
	bool isDead = false;
};
