#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �G
/// </summary>
class Enemy : public ObjObject3d
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
	/// <returns>�G</returns>
	static Enemy* Create(ObjModel* model, const XMFLOAT3& position, const XMFLOAT3& velocity);

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
	/// �ړ�
	/// </summary>
	void Move();

private: //�����o�ϐ�
	//���x
	XMFLOAT3 velocity;
	//�s���t�F�[�Y
	Phase phase = Phase::Previous;
};
