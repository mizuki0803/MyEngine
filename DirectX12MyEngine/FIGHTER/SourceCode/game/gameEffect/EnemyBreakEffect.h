#pragma once
#include "ObjObject3d.h"
#include "GameCamera.h"

/// <summary>
/// �G�j��p�G�t�F�N�g
/// </summary>
class EnemyBreakEffect : public ObjObject3d
{
public: // �ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�G�j��p�G�t�F�N�g</returns>
	static EnemyBreakEffect* Create(ObjModel* model, const Vector3& position, const Vector3& velocity, const Vector3& rotSpeed, const Vector3& scale);

	//setter
	static void SetIsGroundMode(bool isGroundMode) { EnemyBreakEffect::isGroundMode = isGroundMode; }
	static void SetIsGravityMode(bool isGravityMode) { EnemyBreakEffect::isGravityMode = isGravityMode; }
	static void SetGameCamera(GameCamera* gameCamera) { EnemyBreakEffect::gameCamera = gameCamera; }

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	//getter
	bool GetIsDelete() const { return isDelete; }

private: //�����o�֐�
	/// <summary>
	/// �ړ�������
	/// </summary>
	void Move();

	/// <summary>
	/// ��ʎ�O�܂ōs������폜���鏈��
	/// </summary>
	void FrontOfScreenDelete();

protected: //�ÓI�����o�ϐ�
	//�n�ʂ���̍s�����s����
	static bool isGroundMode;
	//�d�͂���̍s�����s����
	static bool isGravityMode;
	//�Q�[���J����
	static GameCamera* gameCamera;

protected: //�����o�ϐ�
	//���x
	Vector3 velocity;
	//��]�̑���
	Vector3 rotSpeed;
	//�폜�t���O
	bool isDelete = false;
};