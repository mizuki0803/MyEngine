#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �X�e�[�W�I��p�f��
/// </summary>
class StageSelectPlanet : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�X�e�[�W�I��p�f��</returns>
	static StageSelectPlanet* Create(ObjModel* model, const Vector3& position);

public:
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;
};
