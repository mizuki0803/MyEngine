#pragma once
#include "Enemy.h"
#include <array>

/// <summary>
/// �����o���G
/// </summary>
class ComeGoEnemy : public Enemy
{
public:
	//�s���t�F�[�Y
	enum class Phase {
		Come,	//����
		Attack,	//�U��
		Go,		//�o��
		Dead,	//���S
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="startPos">�������W</param>
	/// <param name="comePos">�������W</param>
	/// <param name="goPos">�o���ڕW���W</param>
	/// <returns>�����o���G</returns>
	static ComeGoEnemy* Create(const Vector3& startPos, const Vector3& comePos, const Vector3& goTargetPos, const int attackTime);

	//setter
	static void SetAttackMoveSpeed(float moveSpeed) { ComeGoEnemy::attackMoveSpeed = moveSpeed; }
	static void SetModel(ObjModel* model) { ComeGoEnemy::enemyModel = model; }
	static void SetBreakModel(int modelNum, ObjModel* model);

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
	/// ����
	/// </summary>
	void Come();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �o��
	/// </summary>
	void Go();

	/// <summary>
	/// ���S
	/// </summary>
	void Dead();

	/// <summary>
	/// �j��
	/// </summary>
	void Break() override;

private: //�ÓI�����o�ϐ�
	//���ˊԊu
	static const int fireInterval = 120;
	//�s���J��
	static void (ComeGoEnemy::* actionFuncTable[])();
	//�U�����Ɉړ����鑬��
	static float attackMoveSpeed;
	//���f��
	static ObjModel* enemyModel;
	//�j�󎞂ɏo�����f��
	static std::array<ObjModel*, 5> breakModels;

private: //�����o�ϐ�
	//�s��
	Phase phase = Phase::Come;
	//�������W
	Vector3 startPos;
	//�������W
	Vector3 comePos;
	//�������ԃ^�C�}�[
	int32_t comeTimer = 0;
	//�o���J�n���W
	Vector3 goStartPos;
	//�o���ڕW���W
	Vector3 goTargetPos;
	//�o�����ԃ^�C�}�[
	int32_t goTimer = 0;
	//�U�����ԃ^�C�}�[
	int32_t attackTimer = 0;
	//�U������
	int attackTime = 0;
	//���˃^�C�}�[
	int32_t fireTimer = 0;
	//���S���ė����x
	Vector3 crashVel = { 0, 0.05f, 0.05f };
	//���S���ė���]���x
	Vector3 crashRotVel;
};