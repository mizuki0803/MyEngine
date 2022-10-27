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
	static HomingBullet* Create(ObjModel* model, const Vector3& position, const Vector3& velocity, const float size, Enemy* enemy);

	//getter
	static const float GetBlastSize() { return HomingBullet::blastSize; }

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
	void OnCollision() override;

private: //�����o�֐�
	/// <summary>
	/// �n�ʂɏՓ˂����Ƃ�
	/// </summary>
	void CollisionGround() override;

private: //�ÓI�����o�ϐ�
	//��������
	static const int32_t lifeTime = 240;
	//���S���̔���̑傫��
	static const float blastSize;

private: //�����o�ϐ�
	//�����^�C�}�[
	int32_t deathTimer = lifeTime;
	//�z�[�~���O�p�^�C�}�[
	float homingTimer = 0;
};