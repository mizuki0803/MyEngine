#pragma once
#include "ObjObject3d.h"

class Enemy;

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
	Enemy* GetEnemy() { return enemy; }

	void SetEnemy(Enemy* enemy) { this->enemy = enemy; }

protected: //�����o�ϐ�
	//���x
	Vector3 velocity;
	//���S�t���O
	bool isDead = false;
	//�z�[�~���O�p�G
	Enemy* enemy = nullptr;
};
