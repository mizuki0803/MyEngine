#pragma once
#include "EnemyBullet.h"

/// <summary>
/// �{�X�`���[�W�e
/// </summary>
class BossChargeBullet : public EnemyBullet
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�{�X�`���[�W�e</returns>
	static BossChargeBullet* Create(const Vector3& position, const Vector3& velocity, const float size);


public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	void OnCollision() override;
};
