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
	/// <returns>�z�[�~���O�e</returns>
	static HomingBullet* Create(const Vector3& position, const Vector3& velocity, const float size, Enemy* enemy);

	//getter
	static const float GetBlastSize() { return HomingBullet::blastSize; }

	//setter
	static void SetBulletModel(ObjModel* model) { HomingBullet::bulletModel = model; }

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	void OnCollision(const Vector3& subjectPos, float subjectSize, bool isToDamage) override;

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
	//�e�`��p�����f��
	static ObjModel* bulletModel;

private: //�����o�ϐ�
	//�����^�C�}�[
	int32_t deathTimer = lifeTime;
	//�z�[�~���O�p�^�C�}�[
	float homingTimer = 0;
};
