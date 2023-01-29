#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 3D���e�B�N��
/// </summary>
class Reticle3D : public ObjObject3d
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="distance">���ˑΏۂ���̋���</param>
	/// <returns>3D���e�B�N��</returns>
	static Reticle3D* Create(ObjModel* model, float distance);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	//setter
	void SetFollowMatWorld(const XMMATRIX& followMatWorld) { this->followMatWorld = followMatWorld; }

private:
	//�Ǐ]�Ώۂ̃��[���h�s��
	XMMATRIX followMatWorld = {};
	//�Ǐ]�ΏۂƂ̋���
	float distance = 0;
};

