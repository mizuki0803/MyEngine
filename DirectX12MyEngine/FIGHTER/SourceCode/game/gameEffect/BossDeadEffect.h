#pragma once
#include "Vector3.h"

/// <summary>
/// �{�X���S��̃G�t�F�N�g
/// </summary>
class BossDeadEffect
{
public: // �ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="position">�{�X���S���W</param>
	/// <returns>�{�X���S��̃G�t�F�N�g</returns>
	static BossDeadEffect* Create(const Vector3& deadPos);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

private: //�����o�֐�
	/// <summary>
	/// ����
	/// </summary>
	void Explosion();

	/// <summary>
	/// ����
	/// </summary>
	void BlackSmoke();

private: //�����o�ϐ�
	//�{�X���S���W
	Vector3 deadPos;
	//�o�ߎ���
	int32_t timer = 0;
	//������
	int explosionCount = 0;
	//�폜�t���O
	bool isDelete = false;
};