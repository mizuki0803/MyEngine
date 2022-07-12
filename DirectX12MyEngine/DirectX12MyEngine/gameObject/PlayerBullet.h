#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ���@�e
/// </summary>
class PlayerBullet : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>���@�e</returns>
	static PlayerBullet* Create(ObjModel* model, const Vector3& position, const Vector3& velocity);

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
	void OnCollision();

	//getter
	Vector3 GetWorldPos();
	bool GetIsDead() const { return isDead; }

private: //�ÓI�����o�ϐ�
	//��������
	static const int32_t lifeTime = 60;

private: //�����o�ϐ�
	//���x
	Vector3 velocity;
	//�����^�C�}�[
	int32_t deathTimer = lifeTime;
	//���S�t���O
	bool isDead = false;
};
