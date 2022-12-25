#pragma once
#include "Enemy.h"
#include <array>

/// <summary>
/// 覐ΓG(�j��\)
/// </summary>
class MeteoriteEnemy : public Enemy
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="rotation">��]�p</param>
	/// <param name="size">�T�C�Y</param>
	/// <param name="velocity">�ړ����x</param>
	/// <param name="rotSpeed">��]���x</param>
	/// <returns>覐�(�j��s�w�i�p)</returns>
	static MeteoriteEnemy* Create(const Vector3& position, const Vector3& rotation, float size, const Vector3& velocity, const Vector3& rotSpeed, int HP);

	//setter
	static void SetModel(ObjModel* model) { MeteoriteEnemy::meteoriteModel = model; }

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
	/// ���S
	/// </summary>
	void Dead();

	/// <summary>
	/// �j��
	/// </summary>
	void Break() override;

private: //�ÓI�����o�ϐ�
	//���f��
	static ObjModel* meteoriteModel;

private: //�����o�ϐ�
	//���x
	Vector3 velocity = {};
	//��]���x
	Vector3 rotSpeed = {};
	//���S��p�^�C�}�[
	int32_t deadTimer = 0;

};