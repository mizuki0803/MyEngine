#pragma once
#include "Enemy.h"

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
	static ComeGoEnemy* Create(ObjModel* model, const Vector3& startPos, const Vector3& comePos, const Vector3& goTargetPos, const int attackTime);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

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

private: //�ÓI�����o�ϐ�
	//���ˊԊu
	static const int fireInterval = 120;
	//�s���J��
	static void (ComeGoEnemy::* actionFuncTable[])();

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
};