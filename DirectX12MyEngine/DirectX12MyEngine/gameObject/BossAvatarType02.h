#pragma once
#include "BossAvatar.h"

/// <summary>
/// �{�X(���g�F�^�C�v02)
/// </summary>
class BossAvatarType02 : public BossAvatar
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X(���g�F�^�C�v02)</returns>
	static BossAvatarType02* Create(ObjModel* model, ObjObject3d* parent, const Vector3& position);

public: //�����o�֐�
	/// <summary>
	/// �U�����eB�̃��b�N�I������
	/// </summary>
	void AttackTypeBLockon() override;
};
