#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �G�j��p�G�t�F�N�g
/// </summary>
class EnemyBreakEffect : public ObjObject3d
{
public: // �ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�G�j��p�G�t�F�N�g</returns>
	static EnemyBreakEffect* Create(ObjModel* model, const Vector3& position, const Vector3& velocity, const Vector3& rotSpeed, const Vector3& scale);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	//getter
	bool GetIsDelete() const { return isDelete; }

private: //�����o�֐�
	/// <summary>
	/// �ړ�������
	/// </summary>
	void Move();

protected: //�����o�ϐ�
	//���x
	Vector3 velocity;
	//��]�̑���
	Vector3 rotSpeed;
	//�폜�t���O
	bool isDelete = false;
};