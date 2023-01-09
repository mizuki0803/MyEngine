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
	/// <param name="explosionSize">�����̑傫��</param>
	/// <param name="isBlackSmoke">�������o����</param>
	/// <param name="isGround">�n�ʗp�̔��������邩</param>
	/// <returns>�{�X���S��̃G�t�F�N�g</returns>
	static BossDeadEffect* Create(const Vector3& deadPos, const float explosionSize,  const bool isBlackSmoke, const bool isGround);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	//getter
	const bool GetIsExplosionEnd() { return isExplosionEnd; }


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
	//�����̑傫��
	float explosionSize = 0;
	//������
	int explosionCount = 0;
	//�n�ʗp������
	bool isGround = false;
	//�������I��������
	bool isExplosionEnd = false;
	//�������o����
	bool isBlackSmoke = false;
};