#pragma once
#include "BossAvatar.h"

/// <summary>
/// �{�X(���g�F�^�C�v04)
/// </summary>
class BossAvatarType04 : public BossAvatar
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X(���g�F�^�C�v04)</returns>
	static BossAvatarType04* Create(ObjModel* model, ObjObject3d* parent, const Vector3& position);
};
