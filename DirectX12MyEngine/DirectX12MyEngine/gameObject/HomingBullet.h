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

private: //�����o�ϐ�
	//�z�[�~���O�p�^�C�}�[
	float homingTimer = 0;
};
