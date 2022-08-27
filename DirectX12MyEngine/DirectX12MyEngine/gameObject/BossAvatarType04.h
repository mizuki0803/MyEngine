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

public: //�����o�֐�
	/// <summary>
	/// �U�����eB�̃��b�N�I������
	/// </summary>
	void AttackTypeBLockon() override;

	/// <summary>
	/// �U�����eC�̒��S�Ɉړ������鏈��
	/// </summary>
	void AttackTypeCMoveCenter() override;

	/// <summary>
	/// �U�����eD�̑ҋ@����
	/// </summary>
	void AttackTypeDWait() override;

	/// <summary>
	/// �U�����eD�̈ړ������鏈��
	/// </summary>
	void AttackTypeDMove() override;
};
