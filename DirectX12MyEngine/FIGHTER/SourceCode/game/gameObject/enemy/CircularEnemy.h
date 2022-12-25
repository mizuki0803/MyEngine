#pragma once
#include "Enemy.h"
#include <array>

/// <summary>
/// �~�^���G
/// </summary>
class CircularEnemy : public Enemy
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�~�^���G</returns>
	static CircularEnemy* Create(const Vector3& centerPosition, const float angle, const float length, const float rotSpeed);

	//setter
	static void SetModel(ObjModel* model) { CircularEnemy::enemyModel = model; }
	static void SetBreakModel(int modelNum, ObjModel* model);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	void OnCollision(const int damageNum) override;

private: //�����o�֐�
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �j��
	/// </summary>
	void Break() override;

private: //�ÓI�����o�ϐ�
	//�ő�HP
	static const int maxHP = 1;
	//���ˊԊu
	static const int fireInterval = 180;
	//���f��
	static ObjModel* enemyModel;
	//�j�󎞂ɏo�����f��
	static std::array<ObjModel*, 5> breakModels;

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