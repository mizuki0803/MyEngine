#pragma once
#include "Enemy.h"
#include <array>

/// <summary>
/// �~���G
/// </summary>
class FallEnemy : public Enemy
{
public:
	//�s���t�F�[�Y
	enum class Phase {
		Fall,	//�~��
		Rotate,	//��]
		Dead,	//���S
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="stayPos">��~���W</param>
	/// <param name="fallNum">�~����</param>
	/// <returns>�~���G</returns>
	static FallEnemy* Create(const Vector3& stayPos, const float fallNum);

	//setter
	static void SetModel(ObjModel* model) { FallEnemy::enemyModel = model; }
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
	/// �~��
	/// </summary>
	void Fall();

	/// <summary>
	/// ��]
	/// </summary>
	void Rotate();

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
	//�s���J��
	static void (FallEnemy::* actionFuncTable[])();
	//���f��
	static ObjModel* enemyModel;
	//�j�󎞂ɏo�����f��
	static std::array<ObjModel*, 5> breakModels;

private: //�����o�ϐ�
	//�s��
	Phase phase = Phase::Fall;
	//��~���W
	Vector3 stayPos;
	//�~������l
	float fallNum = 0;
	//�~�����鎞�ԃ^�C�}�[
	int32_t fallTimer = 0;
	//���S���ė����x
	Vector3 crashVel = { 0, 0.05f, 0 };
	//���S���ė���]���x
	Vector3 crashRotVel;
};