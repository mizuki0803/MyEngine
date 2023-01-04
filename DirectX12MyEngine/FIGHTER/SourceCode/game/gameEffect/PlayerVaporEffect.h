#pragma once
#include "Vector3.h"
#include <list>

/// <summary>
/// ���@�̔�s�@�_�G�t�F�N�g
/// </summary>
class PlayerVaporEffect
{
private: //��s�@�_�̑傫���ύX���
	enum class VaporSizeChangeMode {
		Increase, //�傫������
		Smaller, //����������
	};

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

	//setter
	void SetIsScrollMode(const bool isScrollMode) { this->isScrollMode = isScrollMode; }

private: //�����o�֐�
	/// <summary>
	/// ��s�@�_����
	/// </summary>
	/// <param name="wingPosList">���̒��߂̍��W���X�g</param>
	/// <param name="segmentCount">������</param>
	void VaporCreate(std::list<Vector3>& wingPosList, const int segmentCount);

	/// <summary>
	/// ��s�@�_�̑傫����ύX
	/// </summary>
	void ChangeVaporSize();

private: //�ÓI�����o�ϐ�
	//�X�N���[�����x
	static const float scrollSpeed;

private: //�����o�ϐ�
	//��s�@�_�𐶐����邩
	bool isVaporCreate = false;
	//��s�@�_�̑傫���ύX���
	VaporSizeChangeMode vaporSizeChangeMode = VaporSizeChangeMode::Increase;
	//����3�t���[�����̍����̍��W
	std::list<Vector3> leftWingPosList;
	//����3�t���[�����̉E���̍��W
	std::list<Vector3> rightWingPosList;
	//��s�@�_�̑傫��
	float vaporSize = 0;
	//�X�N���[����Ԃ�
	bool isScrollMode = false;
};