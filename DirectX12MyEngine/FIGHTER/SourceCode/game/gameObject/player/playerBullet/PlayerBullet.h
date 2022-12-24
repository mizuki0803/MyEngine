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

public: //�ÓI�����o�֐�
	//setter
	static void SetIsGroundMode(bool isGroundMode) { PlayerBullet::isGroundMode = isGroundMode; }

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	virtual void OnCollision(const Vector3& subjectPos, float subjectSize, bool isToDamage = true) = 0;

	//getter
	Vector3 GetWorldPos();
	BulletType GetBulletType() { return bulletType; }
	const Vector3& GetVelocity() { return velocity; }
	bool GetIsDead() const { return isDead; }
	Enemy* GetEnemy() { return enemy; }

	void SetEnemy(Enemy* enemy) { this->enemy = enemy; }

protected: //�����o�֐�
	/// <summary>
	/// �n�ʂɏՓ˂����Ƃ�
	/// </summary>
	virtual void CollisionGround() = 0;

protected: //�ÓI�����o�ϐ�
	//�n�ʂ���̍s�����s����
	static bool isGroundMode;

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
