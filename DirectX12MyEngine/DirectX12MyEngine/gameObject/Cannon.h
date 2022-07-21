#pragma once
#include "Enemy.h"

/// <summary>
/// �G:��C
/// </summary>
class Cannon : public Enemy
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>��C</returns>
	static Cannon* Create(ObjModel* model, const Vector3& position);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�ÓI�����o�ϐ�
	//���ˊԊu
	static const int fireInterval = 300;

private: //�����o�ϐ�
	//���˃^�C�}�[
	int32_t fireTimer = 0;
};