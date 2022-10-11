#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �^�C�g���V�[���p���@
/// </summary>
class TitlePlayer : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�^�C�g���V�[���p���@</returns>
	static TitlePlayer* Create(ObjModel* model, const Vector3& startPosition);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	//getter
	bool GetIsSortie() { return isSortie; }

	//setter
	void SetIsSortie(bool isSortie) { this->isSortie = isSortie; }

private: //�����o�֐�
	/// <summary>
	/// �o��
	/// </summary>
	void Sortie();

private: //�����o�ϐ�
	//�o�����邩
	bool isSortie = false;
	//�o���J�n�܂ŃG���W�����ӂ������ԃ^�C�}�[
	int32_t sortieStartTimer = 0;
};
