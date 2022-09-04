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
	/// <returns>�{�X(���g�F�^�C�v04)</returns>
	static BossAvatarType04* Create(ObjObject3d* parent, const Vector3& position);

public: //�����o�֐�
	/// <summary>
	/// �U�����e:���g�̓�����̃��b�N�I������
	/// </summary>
	void AttackTypeAvatarBodyBlowLockon() override;

	/// <summary>
	/// �U�����e:���g�K�g�����O�C�̒��S�Ɉړ������鏈��
	/// </summary>
	void AttackTypeAvatarGatlingMoveCenter() override;

	/// <summary>
	/// �U�����e:���g����e�̑ҋ@����
	/// </summary>
	void AttackTypeAvatarGiantBulletWait() override;

	/// <summary>
	/// �U�����e:���g����e�̈ړ������鏈��
	/// </summary>
	void AttackTypeAvatarGiantBulletMove() override;
};
