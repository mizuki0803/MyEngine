#pragma once
#include "MedamanAvatar.h"

/// <summary>
/// ���_�}�[��(���g�F�^�C�v02)
/// </summary>
class MedamanAvatarType02 : public MedamanAvatar
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="parent">�e�I�u�W�F�N�g</param>
	/// <param name="position">���W</param>
	/// <returns>���_�}�[��(���g�F�^�C�v02)</returns>
	static MedamanAvatarType02* Create(ObjObject3d* parent, const Vector3& position);

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
