#pragma once
#include "PlayerBullet.h"

/// <summary>
/// ���@�e(���i�e)
/// </summary>
class StraightBullet : public PlayerBullet
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>���i�e</returns>
	static StraightBullet* Create(ObjModel* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�ÓI�����o�ϐ�
	//��������
	static const int32_t lifeTime = 300;

private: //�����o�ϐ�
	//�����^�C�}�[
	int32_t deathTimer = lifeTime;
};
