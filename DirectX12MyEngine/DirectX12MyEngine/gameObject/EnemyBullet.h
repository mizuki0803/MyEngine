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
	static EnemyBullet* Create(ObjModel* model, XMFLOAT3& position, XMFLOAT3& velocity);

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

	//getter
	bool GetIsDead() const { return isDead; }

private: //�ÓI�����o�ϐ�
	//��������
	static const int32_t lifeTime = 180;

private: //�����o�ϐ�
	//���x
	XMFLOAT3 velocity;
	//�����^�C�}�[
	int32_t deathTimer = lifeTime;
	//���S�t���O
	bool isDead = false;
};