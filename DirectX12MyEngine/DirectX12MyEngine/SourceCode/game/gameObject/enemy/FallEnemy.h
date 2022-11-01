#pragma once
#include "Enemy.h"

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
	/// <param name="model">���f��</param>
	/// <returns>�~���G</returns>
	static FallEnemy* Create(ObjModel* model, const Vector3& stayPos, const float fallNum);

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

private: //�ÓI�����o�ϐ�
	//�s���J��
	static void (FallEnemy::* actionFuncTable[])();

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