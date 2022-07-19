#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �G�e
/// </summary>
class EnemyBullet : public ObjObject3d
{
public: // �ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�G�e</returns>
	static EnemyBullet* Create(ObjModel* model, Vector3& position, Vector3& velocity);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
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
	static const int32_t lifeTime = 180;

private: //�����o�ϐ�
	//���x
	Vector3 velocity;
	//�����^�C�}�[
	int32_t deathTimer = lifeTime;
	//���S�t���O
	bool isDead = false;
};