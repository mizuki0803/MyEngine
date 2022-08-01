#pragma once
#include "PlayerBullet.h"

/// <summary>
/// ���@�e(�z�[�~���O�e)
/// </summary>
class HomingBullet : public PlayerBullet
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�z�[�~���O�e</returns>
	static HomingBullet* Create(ObjModel* model, const Vector3& position, const Vector3& velocity, Enemy* enemy);

	/// <summary>
	/// ������
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�ÓI�����o�ϐ�
	//��������
	static const int32_t lifeTime = 240;

private: //�����o�ϐ�
	//�����^�C�}�[
	int32_t deathTimer = lifeTime;
	//�z�[�~���O�p�^�C�}�[
	float homingTimer = 0;
};
