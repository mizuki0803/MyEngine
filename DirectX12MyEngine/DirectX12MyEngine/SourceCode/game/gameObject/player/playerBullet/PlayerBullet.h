#pragma once
#include "ObjObject3d.h"

class Enemy;

/// <summary>
/// ���@�e
/// </summary>
class PlayerBullet : public ObjObject3d
{
public: //enum
	enum class BulletType {
		None,		//���ݒ�
		Straight,	//���i�e
		Homing,		//�z�[�~���O�e
	};

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	virtual void OnCollision(float subjectSize) = 0;

	//getter
	Vector3 GetWorldPos();
	BulletType GetBulletType() { return bulletType; }
	bool GetIsDead() const { return isDead; }
	Enemy* GetEnemy() { return enemy; }

	void SetEnemy(Enemy* enemy) { this->enemy = enemy; }

protected: //�����o�֐�
	/// <summary>
	/// �n�ʂɏՓ˂����Ƃ�
	/// </summary>
	virtual void CollisionGround() = 0;

protected: //�����o�ϐ�
	//�e�̎��
	BulletType bulletType = BulletType::None;
	//���x
	Vector3 velocity;
	//���S�t���O
	bool isDead = false;
	//�z�[�~���O�p�G
	Enemy* enemy = nullptr;
};
