#pragma once
#include "Enemy.h"

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
	/// <param name="model">���f��</param>
	/// <returns>�㉺�ړ��G</returns>
	static UpDownEnemy* Create(ObjModel* model, const Vector3& position);

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

private: //�ÓI�����o�ϐ�
	//���ˊԊu
	static const int fireInterval = 180;
	//�s���J��
	static void (UpDownEnemy::* actionFuncTable[])();

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