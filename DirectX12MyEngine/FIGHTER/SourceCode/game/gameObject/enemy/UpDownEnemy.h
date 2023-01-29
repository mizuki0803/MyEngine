#pragma once
#include "Enemy.h"
#include <array>

/// <summary>
/// �㉺�ړ��G
/// </summary>
class UpDownEnemy : public Enemy
{
public:
	//�s���t�F�[�Y
	enum class Phase {
		UpBrake,	//�㏸�Ƀu���[�L��������
		DownBrake,	//���~�Ƀu���[�L��������
		Dead,		//���S
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="position">���W</param>
	/// <returns>�㉺�ړ��G</returns>
	static UpDownEnemy* Create(const Vector3& position);

	//setter
	static void SetModel(ObjModel* model) { UpDownEnemy::enemyModel = model; }
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
	/// �㏸�Ƀu���[�L��������
	/// </summary>
	void UpBrake();

	/// <summary>
	/// ���~�Ƀu���[�L��������
	/// </summary>
	void DownBrake();

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
	static const int fireInterval = 100;
	//�s���J��
	static void (UpDownEnemy::* actionFuncTable[])();
	//���f��
	static ObjModel* enemyModel;
	//�j�󎞂ɏo�����f��
	static std::array<ObjModel*, 5> breakModels;

private: //�����o�ϐ�
	//�s��
	Phase phase = Phase::UpBrake;
	//���x
	Vector3 velocity = { 0, 1.0f, -0.02f };
	//���˃^�C�}�[
	int32_t fireTimer = 0;
	//���S���ė����x
	Vector3 crashVel = { 0, 0.5f, 0.5f };
	//���S���ė���]���x
	Vector3 crashRotVel;
};