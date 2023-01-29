#pragma once
#include "EnemyBullet.h"

/// <summary>
/// �G�Ή��e
/// </summary>
class EnemyFireBullet : public EnemyBullet
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>	
	/// <param name="position">���W</param>
	/// <param name="velocity">���x</param>
	/// <param name="size">�傫��</param>
	/// <returns>�G�Ή��e</returns>
	static EnemyFireBullet* Create(const Vector3& position, const Vector3& velocity, const float size);


public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	void OnCollision() override;

private: //�ÓI�����o�ϐ�
	//��������
	static const int32_t lifeTime = 90;
};
