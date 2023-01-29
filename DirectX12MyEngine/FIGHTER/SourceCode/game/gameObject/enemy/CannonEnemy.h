#pragma once
#include "Enemy.h"
#include <array>

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
	/// <param name="position">���W</param>
	/// <returns>��C�G</returns>
	static CannonEnemy* Create(const Vector3& position);

	//setter
	static void SetModel(ObjModel* model) { CannonEnemy::enemyModel = model; }
	static void SetBreakModel(int modelNum, ObjModel* model);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="damageNum">�_���[�W��</param>
	void OnCollision(const int damageNum) override;

private: //�����o�֐�
	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// ���S
	/// </summary>
	void Dead();

	/// <summary>
	/// �j��
	/// </summary>
	void Break() override;

private: //�ÓI�����o�ϐ�
	//�ő�HP
	static const int maxHP = 1;
	//���ˊԊu
	static const int fireInterval = 300;
	//�s���J��
	static void (CannonEnemy::* actionFuncTable[])();
	//���f��
	static ObjModel* enemyModel;
	//�j�󎞂ɏo�����f��
	static std::array<ObjModel*, 5> breakModels;

private: //�����o�ϐ�
	//�s��
	Phase phase = Phase::Attack;
	//���˃^�C�}�[
	int32_t fireTimer = 0;
	//���S���ė����x
	Vector3 crashVel = { 0, 0.1f, 0.0025f };
	//���S���ė���]���x
	Vector3 crashRotVel;
};