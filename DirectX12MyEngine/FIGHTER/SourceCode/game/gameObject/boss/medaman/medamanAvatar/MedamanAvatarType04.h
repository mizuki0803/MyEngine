#pragma once
#include "MedamanAvatar.h"

/// <summary>
/// ���_�}�[��(���g�F�^�C�v04)
/// </summary>
class MedamanAvatarType04 : public MedamanAvatar
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>���_�}�[��(���g�F�^�C�v04)</returns>
	static MedamanAvatarType04* Create(ObjObject3d* parent, const Vector3& position);

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
