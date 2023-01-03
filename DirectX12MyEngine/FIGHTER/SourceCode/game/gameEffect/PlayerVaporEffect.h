#pragma once
#include "Vector3.h"
#include <list>

/// <summary>
/// ���@�̔�s�@�_�G�t�F�N�g
/// </summary>
class PlayerVaporEffect
{
public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update(const Vector3& leftWingPos, const Vector3& rightWingPos);

	/// <summary>
	/// ��s�@�_���o���J�n����
	/// </summary>
	void VaporStart();

	/// <summary>
	/// ��s�@�_���o���I������
	/// </summary>
	void VaporEnd();

private: //�����o�֐�
	/// <summary>
	/// ��s�@�_����
	/// </summary>
	/// <param name="wingPosList">���̒��߂̍��W���X�g</param>
	/// <param name="segmentCount">������</param>
	void VaporCreate(std::list<Vector3>& wingPosList, const int segmentCount);

private: //�����o�ϐ�
	//����3�t���[�����̍����̍��W
	std::list<Vector3> leftWingPosList;
	//����3�t���[�����̉E���̍��W
	std::list<Vector3> rightWingPosList;
};