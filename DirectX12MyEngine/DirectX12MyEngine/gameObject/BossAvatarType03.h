#pragma once
#include "BossAvatar.h"

/// <summary>
/// �{�X(���g�F�^�C�v03)
/// </summary>
class BossAvatarType03 : public BossAvatar
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X(���g�F�^�C�v03)</returns>
	static BossAvatarType03* Create(ObjModel* model, ObjObject3d* parent, const Vector3& position);

public: //�����o�֐�
	/// <summary>
	/// �U�����eB�̃��b�N�I������
	/// </summary>
	void AttackTypeBLockon() override;
};
