#pragma once
#include "Enemy.h"

/// <summary>
/// �~�^���G
/// </summary>
class CircularEnemy : public Enemy
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�~�^���G</returns>
	static CircularEnemy* Create(ObjModel* model, const Vector3& centerPosition, const float angle, const float length, const float rotSpeed);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

private: //�ÓI�����o�ϐ�
	//���ˊԊu
	static const int fireInterval = 180;

private: //�����o�ϐ�
	//�~�^���̒��S���W
	Vector3 centerPosition;
	//�~�^���p�p�x
	float angle;
	//�~�̔��a�̒���
	float length;
	//��]���x
	float rotSpeed;
	//���˃^�C�}�[
	int32_t fireTimer = 0;
};