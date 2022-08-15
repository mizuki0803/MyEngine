#pragma once
#include "Enemy.h"

/// <summary>
/// �m�F�p�G
/// </summary>
class DemoEnemy : public Enemy
{
public:
	//�s���t�F�[�Y
	enum class Phase {
		Previous,	//�O�i
		Back,		//���
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�m�F�p�G</returns>
	static DemoEnemy* Create(ObjModel* model, const Vector3& position, const Vector3& velocity);

public: //�����o�֐�
/// <summary>
/// ������
/// </summary>
	bool Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;


private: //�����o�֐�
	/// <summary>
	/// �O�i
	/// </summary>
	void Previous();

	/// <summary>
	/// ���
	/// </summary>
	void Back();

private: //�ÓI�����o�ϐ�
	//���ˊԊu
	static const int fireInterval = 60;
	//�s���J��
	static void (DemoEnemy::* actionFuncTable[])();

private: //�����o�ϐ�
	//���x
	Vector3 velocity;
	//�s���t�F�[�Y
	Phase phase = Phase::Previous;
	//���˃^�C�}�[
	int32_t fireTimer = 0;
};
