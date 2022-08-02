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
		Shot,	//�ˌ�
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�~���G</returns>
	static FallEnemy* Create(ObjModel* model, const Vector3& position);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	/// �s��
	/// </summary>
	void Action();

private: //�ÓI�����o�ϐ�
	//���ˊԊu
	static const int fireInterval = 180;

private: //�����o�ϐ�
	//�s��
	Phase phase = Phase::Fall;
	//�������W
	Vector3 startPos;
	//�~�����鎞�ԃ^�C�}�[
	int32_t fallTimer = 0;
	//���˃^�C�}�[
	int32_t fireTimer = 0;
};