#pragma once
#include "Enemy.h"

/// <summary>
/// ��C�G
/// </summary>
class CannonEnemy : public Enemy
{
public:
	//�s���t�F�[�Y
	enum class Phase {
		Attack,	//�U��
		Dead,	//���S
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>��C�G</returns>
	static CannonEnemy* Create(const Vector3& position);

	//setter
	static void SetModel(ObjModel* model) { CannonEnemy::enemyModel = model; }
	static void SetBreakModel(ObjModel* model) { CannonEnemy::breakModel = model; }

public: //�����o�֐�
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
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// ���S
	/// </summary>
	void Dead();

private: //�ÓI�����o�ϐ�
	//���ˊԊu
	static const int fireInterval = 300;
	//�s���J��
	static void (CannonEnemy::* actionFuncTable[])();
	//���f��
	static ObjModel* enemyModel;
	//�j�󂳂ꂽ���f��
	static ObjModel* breakModel;

private: //�����o�ϐ�
	//�s��
	Phase phase = Phase::Attack;
	//���˃^�C�}�[
	int32_t fireTimer = 0;
};